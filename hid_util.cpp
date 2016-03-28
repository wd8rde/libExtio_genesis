#include <pthread.h>
#include "hidapi.h"
#include "hid_util.h"

static pthread_mutex_t m_hid_mutex = PTHREAD_MUTEX_INITIALIZER;

hid_handle open_device(unsigned short vendor_id, unsigned short product_id, const wchar_t *p_serial_number)
{
    pthread_mutex_lock(&m_hid_mutex);
    hid_handle handle = (hid_handle) hid_open(vendor_id, product_id, NULL);
    pthread_mutex_unlock(&m_hid_mutex);
    return handle;
}

void close_device(hid_handle handle)
{
    pthread_mutex_lock(&m_hid_mutex);
    hid_device* p_device = (hid_device*)handle;
    hid_close(p_device);
    pthread_mutex_unlock(&m_hid_mutex);
}

int write_to_device(hid_handle handle, const unsigned char *p_data, size_t length)
{
    int rtn = 0;
    pthread_mutex_lock(&m_hid_mutex);
    hid_device* p_device = (hid_device*)handle;
    rtn = hid_write(p_device, p_data, length);
    pthread_mutex_unlock(&m_hid_mutex);
    return rtn;
}

int read_from_device(hid_handle handle, unsigned char *p_data, size_t length)
{
    int rtn = 0;
    hid_device* p_device = (hid_device*)handle;
    pthread_mutex_lock(&m_hid_mutex);
    hid_set_nonblocking(p_device, 1);
    rtn = hid_read (p_device, p_data, length);
    pthread_mutex_unlock(&m_hid_mutex);
    return rtn;
}

int get_feature_report(hid_handle handle, unsigned char *p_data, size_t length)
{
    int rtn = 0;
    hid_device* p_device = (hid_device*)handle;
    pthread_mutex_lock(&m_hid_mutex);
  	// Read a Feature Report from the device
	p_data[0] = 0x2;
    rtn = hid_get_feature_report(p_device, p_data, length);
    pthread_mutex_unlock(&m_hid_mutex);
	return rtn;

}
