#ifndef USBPROGRAMMER_H
#define USBPROGRAMMER_H

#include "programmer.h"
#include "usbdriver.h"
#include "stdint.h"

/* This file servers as usbspi.dll programmer interface
 * and partially as pttransport.dll transport layer
 */

#define CSRVENDOR 0x0a12
#define CSRDEVICE 0x0042

class UsbProgrammer : public Programmer
{
public:
    UsbProgrammer();
    bool ReadBlock(uint16_t addr, int size, uint16_t buffer[]);
    bool WriteBlock(uint16_t addr, int size, uint16_t buffer[]);
    bool SetTransferSpeed(uint16_t speedkhz);
    bool IsXAPStopped(void);
    bool IsConnected(void);

private:
    bool SetMode(bool spi);
    bool ClearCmdBits();

private:
    UsbDriver usb;
    bool progInit;
};

#endif // USBPROGRAMMER_H
