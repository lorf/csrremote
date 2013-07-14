#include <iostream>
#include "programmer.h"
#include "usbprogrammer.h"
#include "remote.h"

using namespace std;

int main()
{
    cout << "CSR Remote Programmer server for ver. 0x103" << endl;

    Programmer *prog = new UsbProgrammer();

    if(!prog->IsConnected())
    {
        cout << "Programmer is not connected!" << endl;
        delete prog;
        return 1;
    }

    Remote server(prog);
    while(1)
    {
        if(server.WaitForClient())
        {
            cout << "Client connected." << endl;
            server.Run();
            server.Disconnect();
        } else
            break;
    }

    delete prog;

    return 0;
}

