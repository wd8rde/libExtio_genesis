#ifndef _HID_UTIL_H
#define _HID_UTIL_H

typedef void* hid_handle;

hid_handle open_device(unsigned short vendor_id, unsigned short product_id, const wchar_t *p_serial_number);
void close_device(hid_handle handle);
int write_to_device(hid_handle handle, const unsigned char *p_data, size_t length);
int read_from_device(hid_handle handle, unsigned char *p_data, size_t length);
int get_feature_report(hid_handle handle, unsigned char *p_data, size_t length);
#endif /* _HID_UTIL_H */

