// vim: sw=8:ts=8:noexpandtab
#include <stdio.h>
#include <pthread.h>

#include "usbutil.h"

#define DUMP_STRUCTURES
#ifdef DUMP_STRUCTURES
#define DUMP_DEVICE_DESCRIPTOR(...) usbutil_dump_device_descriptor(__VA_ARGS__)
#define DUMP_CONFIG_DESCRIPTOR(...) usbutil_dump_config_descriptor(__VA_ARGS__)
#else
#define DUMP_DEVICE_DESCRIPTOR(...)
#define DUMP_CONFIG_DESCRIPTOR(...)
#endif
#define LOG(...) { fprintf(stderr,__VA_ARGS__); fflush(stderr); }


//static pthread_mutex_t s_usb_mutex = PTHREAD_MUTEX_INITIALIZER; 

int write_to_device(libusb_device_handle *dev_handle, unsigned char endpoint, unsigned char *data, int length)
{
    //pthread_mutex_lock(&s_usb_mutex);
    /* Send the message to endpoint 1 with a 100ms timeout. */
    int numBytes = 0;
    const unsigned int TIMEOUT = 1000;
    int res = libusb_interrupt_transfer(dev_handle, endpoint, data, length, &numBytes, TIMEOUT);
    if(res == 0)
    {
        LOG("%d bytes transmitted successfully.\n", numBytes);
    } else
    {
        LOG("Error: sending message to device, %s:%s.\n", libusb_error_name(res), libusb_strerror(static_cast<libusb_error>(res)));
    }

    //pthread_mutex_unlock(&s_usb_mutex);
    return  (numBytes);
}

int read_from_device(libusb_device_handle *dev_handle, unsigned char endpoint, unsigned char *data, int length)
{
    //pthread_mutex_lock(&s_usb_mutex);
    /* Send the message to endpoint 1 with a 100ms timeout. */
    int numBytes = 0;
    const unsigned int TIMEOUT = 100;
    int res = libusb_interrupt_transfer(dev_handle, endpoint, data, length, &numBytes, TIMEOUT);
    if(res == 0)
    {
        LOG("%d bytes received successfully.\n", numBytes);
    } else if ( LIBUSB_ERROR_TIMEOUT != res )
    {
        LOG("Error retrieving message from device, %s:%s.\n", libusb_error_name(res), libusb_strerror(static_cast<libusb_error>(res)));
    }

    //pthread_mutex_unlock(&s_usb_mutex);
    return  (numBytes);
}

/*
 * Data structure debugging.
 */
static void dump_endpoint_descriptor(FILE *file, int i, const struct libusb_endpoint_descriptor *endpoint_descriptor)
{
    const char *direction = ((endpoint_descriptor->bEndpointAddress & 0x80) == LIBUSB_ENDPOINT_IN) ? "in" : "out";

    fprintf(file, "\t\t\tEndpoint #%d\n", i);
    fprintf(file, "\t\t\t\tAddress: %d, direction=%s\n", endpoint_descriptor->bEndpointAddress & 0x0f, direction);
    fprintf(file, "\t\t\t\tAttributes: %02x\n", endpoint_descriptor->bmAttributes);
    fprintf(file, "\t\t\t\tMax packet size: %u\n", endpoint_descriptor->wMaxPacketSize);
    fprintf(file, "\t\t\t\tPoll interval: %d\n", endpoint_descriptor->bInterval);
    fprintf(file, "\t\t\t\tRefresh: %d\n", endpoint_descriptor->bRefresh);
    fprintf(file, "\t\t\t\tSync address: %d\n", endpoint_descriptor->bSynchAddress);
}

static void dump_interface(FILE *file, int i, const struct libusb_interface *interface)
{
    fprintf(file, "\t\tInterface #%d: Descriptors: (%d)\n", i, interface->num_altsetting);
    const struct libusb_interface_descriptor *interface_descriptor = interface->altsetting;
    int j, k;
    for (j = 0; j < interface->num_altsetting; j++, interface_descriptor++)
    {
        fprintf(file, "\t\t\tDescriptor #%d:\n", j);
        fprintf(file, "\t\t\tInterface class/sub-class: %d/%d\n",
                interface_descriptor->bInterfaceClass, interface_descriptor->bInterfaceSubClass);
        fprintf(file, "\t\t\tProtocol: %d\n", interface_descriptor->bInterfaceProtocol);
        fprintf(file, "\t\t\tEndpoints: (%d)\n", interface_descriptor->bNumEndpoints);
        const struct libusb_endpoint_descriptor
            *endpoint_descriptor = interface_descriptor->endpoint;
        for (k = 0; k < interface_descriptor->bNumEndpoints; k++, endpoint_descriptor++)
        {
            dump_endpoint_descriptor(file, k, endpoint_descriptor);
        }
    }
}

void usbutil_dump_config_descriptor(FILE *file, struct libusb_config_descriptor *config_descriptor)
{
    fprintf(file, "\tConfiguration id: %d\n", config_descriptor->bConfigurationValue);
    const struct libusb_interface *interface = config_descriptor->interface;
    int i;
    for (i = 0; i < config_descriptor->bNumInterfaces; i++, interface++)
    {
        dump_interface(file, i, interface);
    }
}

void usbutil_dump_device_descriptor(FILE *file, struct libusb_device_descriptor *device_descriptor)
{
    /* TODO: Decode bytes to strings */
    fprintf(file, "Device descriptor:\n");
    fprintf(file, "\tClass/Sub-class: %04x/%04x\n", device_descriptor->bDeviceClass,
            device_descriptor->bDeviceSubClass);
    fprintf(file, "\tProtocol: %d\n", device_descriptor->bDeviceProtocol);
    fprintf(file, "\tVendor id / product id: %04x / %04x\n", device_descriptor->idVendor,
            device_descriptor->idProduct);
}

/*
 * Open / Close
 */

/*
 * This method looks if the kernel already has a driver attached to the device. if so I will take over the device.
 */
static int claim_device(libusb_device_handle *dev, int interface)
{
    int err;

    libusb_release_interface(dev, 0);

    if(libusb_kernel_driver_active(dev, interface))
    {
        LOG("A kernel has claimed the interface, detaching it...\n");
        if((err = libusb_detach_kernel_driver(dev, interface)) != 0)
        {
            LOG("Failed to Disconnected the OS driver: %s\n", usbutil_error_to_string(err));
            return (err);
        }
    }

    if((err = libusb_set_configuration(dev, 1)))
    {
        LOG("libusb_set_configuration: %s\n", usbutil_error_to_string(err));
        return (err);
    }

    /* claim interface */
    if((err = libusb_claim_interface(dev, interface)))
    {
        LOG("Claim interface error: %s\n", usbutil_error_to_string(err));
        return (err);
    }

    if((err = libusb_set_interface_alt_setting(dev, interface, 0)))
    {
        LOG("libusb_set_interface_alt_setting: %s\n", usbutil_error_to_string(err));
        return (err);
    }

    return (LIBUSB_SUCCESS);
}

/*
 * Iterates over the usb devices on the usb busses and returns a handle to the
 * first device found that matches the predefined vendor and product id
 */
libusb_device_handle* open_device(libusb_context *ctx, int vendor_id, int product_id, unsigned char *in_endpoint, unsigned char *out_endpoint, int *interface_idx)
{
    libusb_device_handle *rtn = NULL;
    int found_in_endpoint = 0;
    int found_out_endpoint = 0;

    //pthread_mutex_lock(&s_usb_mutex);
    // discover devices
    libusb_device **list;
    libusb_device *found = NULL;
    libusb_device_handle *device_handle = NULL;
    struct libusb_device_descriptor descriptor;

    size_t cnt = libusb_get_device_list(ctx, &list);
    size_t i = 0;
    int err = 0;
    if(cnt < 0)
    {
        LOG("Failed to get a list of devices\n");
        //pthread_mutex_unlock(&s_usb_mutex);
        return (NULL);
    }

    for (i = 0; i < cnt; i++)
    { 
         libusb_device *device = list[i];
        err = libusb_get_device_descriptor(device, &descriptor);
        if(err)
        {
            LOG("libusb_get_device_descriptor: %s\n", usbutil_error_to_string(err));
            libusb_free_device_list(list, 1);
            //pthread_mutex_unlock(&s_usb_mutex);
            return (NULL);
        }
        if((descriptor.idVendor == vendor_id) && (descriptor.idProduct == product_id))
        {
            found = device;
            DUMP_DEVICE_DESCRIPTOR(stderr, &descriptor);
            break;
        }
    }

    if(!found)
    {
        LOG("Device not found\n");
        libusb_free_device_list(list, 1);
        //pthread_mutex_unlock(&s_usb_mutex);
        return (NULL);
    }

    if((err = libusb_open(found, &device_handle)))
    {
        LOG("Failed OPEN the device: %s\n", usbutil_error_to_string(err));
        libusb_free_device_list(list, 1);
        //pthread_mutex_unlock(&s_usb_mutex);
        return (NULL);
    }

    libusb_free_device_list(list, 1);

    if((err = claim_device(device_handle, 0)) != 0)
    {
        LOG("Failed to claim the usb interface: %s\n", usbutil_error_to_string(err));
        //pthread_mutex_unlock(&s_usb_mutex);
        return (NULL);
    }

    struct libusb_config_descriptor *config_descriptor;
    err = libusb_get_active_config_descriptor(found, &config_descriptor);
    if(err)
    {
        LOG("libusb_get_active_config_descriptor: %s\n", usbutil_error_to_string(err));
        //pthread_mutex_unlock(&s_usb_mutex);
        return (NULL);
    }

    //use first configuration
    DUMP_CONFIG_DESCRIPTOR(stderr, config_descriptor);
    const struct libusb_interface *interface = config_descriptor->interface;
    for (i = 0; i < config_descriptor->bNumInterfaces; i++, interface++)
    {
        *interface_idx = i;
        const struct libusb_interface_descriptor *interface_descriptor = interface->altsetting;
        int j, k;
        for (j = 0; j < interface->num_altsetting; j++, interface_descriptor++)
        {
            const struct libusb_endpoint_descriptor
                *endpoint_descriptor = interface_descriptor->endpoint;
            for (k = 0; k < interface_descriptor->bNumEndpoints; k++, endpoint_descriptor++)
            {
                if(((endpoint_descriptor->bEndpointAddress & 0x80) == LIBUSB_ENDPOINT_IN) &&
                   ((endpoint_descriptor->bmAttributes & 0x03) == LIBUSB_TRANSFER_TYPE_INTERRUPT))
                {
                    *in_endpoint = endpoint_descriptor->bEndpointAddress;
                    found_in_endpoint = 1;
                } else if(((endpoint_descriptor->bEndpointAddress & 0x80) == LIBUSB_ENDPOINT_OUT) &&
                          ((endpoint_descriptor->bmAttributes & 0x03) == LIBUSB_TRANSFER_TYPE_INTERRUPT))
                {
                    *out_endpoint = endpoint_descriptor->bEndpointAddress;
                    found_out_endpoint = 1;
                }
            }
        }
    }
    libusb_free_config_descriptor(config_descriptor);

    if(found_in_endpoint && found_out_endpoint)
    {
        rtn = device_handle;
    }
    //pthread_mutex_unlock(&s_usb_mutex);
    return (rtn);
}

int close_device(libusb_device_handle *dev, int interface_idx)
{
    int err =0;
    //pthread_mutex_lock(&s_usb_mutex);
    LOG("%s:%d\n", __FUNCTION__,__LINE__);
    if(libusb_kernel_driver_active(dev, 0))
    {
        LOG("A kernel has claimed the interface, detaching it...\n");
        if((err = libusb_detach_kernel_driver(dev, 0)) != 0)
        {
            LOG("Failed to Disconnected the OS driver: %s\n", usbutil_error_to_string(err));
        }
    }
    libusb_release_interface(dev, 0);
    //pthread_mutex_unlock(&s_usb_mutex);
    return (err);
}

const char* usbutil_transfer_status_to_string(enum libusb_transfer_status transfer_status)
{
    switch(transfer_status)
    {
    case LIBUSB_TRANSFER_COMPLETED:
        return ("Completed.");
    case LIBUSB_TRANSFER_ERROR:
        return ("Error.");
    case LIBUSB_TRANSFER_TIMED_OUT:
        return ("Timeout.");
    case LIBUSB_TRANSFER_CANCELLED:
        return ("Cancelled.");
    case LIBUSB_TRANSFER_STALL:
        return ("Stalled.");
    case LIBUSB_TRANSFER_NO_DEVICE:
        return ("No device.");
    case LIBUSB_TRANSFER_OVERFLOW:
        return ("Overflow.");
    default:
        return ("libusb_transfer_status: Unkown.");
    }
}

const char* usbutil_error_to_string(int error)
{
    switch(error)
    {
    case LIBUSB_SUCCESS:
        return ("LIBUSB_SUCCESS");
    case LIBUSB_ERROR_IO:
        return ("LIBUSB_ERROR_IO");
    case LIBUSB_ERROR_INVALID_PARAM:
        return ("LIBUSB_ERROR_INVALID_PARAM");
    case LIBUSB_ERROR_ACCESS:
        return ("LIBUSB_ERROR_ACCESS");
    case LIBUSB_ERROR_NO_DEVICE:
        return ("LIBUSB_ERROR_NO_DEVICE");
    case LIBUSB_ERROR_NOT_FOUND:
        return ("LIBUSB_ERROR_NOT_FOUND");
    case LIBUSB_ERROR_BUSY:
        return ("LIBUSB_ERROR_BUSY");
    case LIBUSB_ERROR_TIMEOUT:
        return ("LIBUSB_ERROR_TIMEOUT");
    case LIBUSB_ERROR_OVERFLOW:
        return ("LIBUSB_ERROR_OVERFLOW");
    case LIBUSB_ERROR_PIPE:
        return ("LIBUSB_ERROR_PIPE");
    case LIBUSB_ERROR_INTERRUPTED:
        return ("LIBUSB_ERROR_INTERRUPTED");
    case LIBUSB_ERROR_NO_MEM:
        return ("LIBUSB_ERROR_NO_MEM");
    case LIBUSB_ERROR_NOT_SUPPORTED:
        return ("LIBUSB_ERROR_NOT_SUPPORTED");
    case LIBUSB_ERROR_OTHER:
        return ("LIBUSB_ERROR_OTHER");
    default:
        return ("libusb_error: Unknown error");
    }
}
