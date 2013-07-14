#include "usbdriver.h"

UsbDriver::UsbDriver() :
    initialized(false),
    ctx(NULL),
    dev_handle(NULL)
{

}

bool UsbDriver::USBInit(uint16_t vendor, uint16_t device)
{
    if(initialized) return true;

    int r;
    r = libusb_init(&ctx);

    dev_handle = libusb_open_device_with_vid_pid(ctx, vendor, device);
    if(dev_handle == NULL) {
        USBRelease();
        return false;
    }

    r = libusb_claim_interface(dev_handle, 0);
    if(r < 0) {
        USBRelease();
        return false;
    }

    initialized = true;
    return true;
}

void UsbDriver::USBRelease(void)
{
    if(dev_handle) {
        libusb_release_interface(dev_handle, 0);
        libusb_close(dev_handle);
    }

    if(ctx)
        libusb_exit(ctx);

    ctx = NULL;
    dev_handle = NULL;
    initialized = false;
}

UsbDriver::~UsbDriver()
{
    USBRelease();
}

/*bool UsbDriver::IsInitialized(void)
{
    return initialized;
}*/

bool UsbDriver::WriteData(unsigned char data[], int size)
{
    if(!initialized) return false;

    int r,actual,writed = 0;
    do {
        r = libusb_bulk_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_OUT),
                     &data[writed], size-writed, &actual, 0);
        writed += actual;
        if(r) return false;
    } while (writed < size);
    return true;
}

bool UsbDriver::ReadData(unsigned char data[], int size)
{
    if(!initialized) return false;

    int r,actual = 0,readed = 0;
    do {
        r = libusb_bulk_transfer(dev_handle, (2 | LIBUSB_ENDPOINT_IN),
                    &data[readed], size-readed, &actual, 0);
        readed += actual;
        if(r) return false;
    } while(readed < size);
    return true;
}
