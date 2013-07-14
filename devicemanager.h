#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "programmer.h"

/* This file servers partially as pttransport.dll transport layer */

class DeviceManager
{
public:
    DeviceManager(Programmer *prog);

    bool XapResetAndGo();
    bool XapResetAndStop();
    bool XapGo();
    bool XapStop();
    bool IsSupported();

private:
    Programmer *programmer;
};

#endif // DEVICEMANAGER_H
