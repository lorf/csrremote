#include "programmer.h"

bool Programmer::Read(uint16_t addr, uint16_t *data)
{
    return ReadBlock(addr, 1, data);
}

bool Programmer::Write(uint16_t addr, uint16_t data)
{
    return WriteBlock(addr, 1, &data);
}
