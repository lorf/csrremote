#ifndef PROGRAMMER_H
#define PROGRAMMER_H

#include <stdint.h>

class Programmer
{
public:
    virtual bool Read(uint16_t addr, uint16_t *data);
    virtual bool Write(uint16_t addr, uint16_t data);
    virtual ~Programmer() {}

    //pure virtual
    virtual bool ReadBlock(uint16_t addr, int size, uint16_t buffer[]) = 0;
    virtual bool WriteBlock(uint16_t addr, int size, uint16_t buffer[]) = 0;
    virtual bool SetTransferSpeed(uint16_t speedkhz) = 0;
    virtual bool IsXAPStopped(void) = 0;
    virtual bool IsConnected(void) = 0;
};

#endif // PROGRAMMER_H
