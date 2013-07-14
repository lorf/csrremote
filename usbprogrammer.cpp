#include "usbprogrammer.h"

#define convertEndianess(A) (uint16_t)((((uint16_t)(A) & 0xff00) >> 8) | \
                             (((uint16_t)(A) & 0x00ff) << 8))

#define arr unsigned char[]

#define __packed __attribute__((__packed__))

struct RWPacket{
    uint16_t cmd;
    uint8_t  padding;
    uint16_t addr;
    uint16_t size;
    uint16_t data[505];
} __packed;

UsbProgrammer::UsbProgrammer()
{
    progInit = usb.USBInit(CSRVENDOR,CSRDEVICE);
    SetMode(true);
    SetTransferSpeed(20);
    ClearCmdBits();
}

bool UsbProgrammer::IsConnected(void)
{
    return progInit;
}

bool UsbProgrammer::ReadBlock(uint16_t addr, int size, uint16_t buffer[])
{
    RWPacket packet;
    packet.cmd = 0x100;
    packet.padding = 0;

    int readed = 0;
    unsigned short address = addr;

    do {

        int psize = 0x1F8;
        if(psize+readed > size)
            psize = size - readed;

        packet.addr = convertEndianess(address);
        packet.size = convertEndianess(psize);

        if(!usb.WriteData((unsigned char*)&packet,7)) return false;

        uint16_t tmp[3+psize];
        if(!usb.ReadData((unsigned char*)tmp,6+psize*2)) return false;

        for(int i=readed; i<readed+psize; i++)
            buffer[i] = convertEndianess(tmp[3+i-readed]);

        address += psize;
        readed += psize;

    } while (readed < size);


    return true;
}

bool UsbProgrammer::WriteBlock(uint16_t addr, int size, uint16_t buffer[])
{
    RWPacket packet;
    packet.cmd = 0x200;
    packet.padding = 0;

    int writed = 0;
    unsigned short address = addr;

    do {

        int psize = 0x1F8;
        if(psize+writed > size)
            psize = size - writed;

        packet.addr = convertEndianess(address);
        packet.size = convertEndianess(psize);

        for(int i=writed; i<writed+psize; i++)
            packet.data[i-writed] = convertEndianess(buffer[i]);

        if(!usb.WriteData((unsigned char*)&packet,7+psize*2)) return false;

        address += psize;
        writed += psize;

    } while (writed < size);


    return true;
}

bool UsbProgrammer::SetTransferSpeed(uint16_t speedkhz)
{
    uint16_t speed = (1000000 / speedkhz - 434) / 126;

    uint8_t low = speed;
    uint8_t high = speed >> 8;
    unsigned char cmd[] = {0,3,0,high,low};
    return usb.WriteData(cmd,5);
}

bool UsbProgrammer::IsXAPStopped(void)
{
    uint16_t data[2];
    unsigned char cmd[] = {0,4,0};
    usb.WriteData(cmd,3);
    usb.ReadData((unsigned char*)data,4);
    return data[1];
}

bool UsbProgrammer::SetMode(bool spi)
{
    uint8_t data = spi ? 0 : 0xff;
    unsigned char cmd[] = {0,9,0,0,data};
    return usb.WriteData(cmd,5);
}

bool UsbProgrammer::ClearCmdBits()
{
    unsigned char cmd[] = {0,15,0,0,0,0,0};
    return usb.WriteData(cmd,7);
}
