#ifndef USBUTIL_H_
#define USBUTIL_H_

#include <libusb.h>

/*
 * Iterates over the usb devices on the usb busses and returns a handle to the
 * first device found that matches the predefined vendor and product id
 */
libusb_device_handle *open_device(libusb_context * ctx, int vendor_id, int product_id, unsigned char *in_endpoint, unsigned char *out_endpoint, int *interface_idx);
int close_device(libusb_device_handle *dev, int interface_idx);

const char *usbutil_transfer_status_to_string(enum libusb_transfer_status transfer_status);

const char *usbutil_error_to_string(int error);

int write_to_device(libusb_device_handle *dev_handle, unsigned char endpoint, unsigned char *data, int length);
int read_from_device(libusb_device_handle *dev_handle, unsigned char endpoint, unsigned char *data, int length);
#endif /*USBUTIL_H_*/
