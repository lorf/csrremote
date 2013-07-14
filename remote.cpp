#include "remote.h"

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <cstdlib>

struct retdata {
    int len;
    unsigned short* data;
};

using namespace std;

Remote::Remote(Programmer *prog) : connected(false),
    clock(1000), programmer(prog), manager(prog)
{
    if(programmer == NULL) return;

    programmer->SetTransferSpeed(1000);

    /*WSADATA data;
    WSAStartup(0x202,&data);*/
}

bool Remote::ReceiveData(char* buf, int len)
{
    int readed = 0;
    while (readed < len)
    {
        int r = recv(clientfd,&buf[readed],len,0);
        if(r == 0 || r == -1)
            return false;
        readed += r;
    }
    return true;
}

bool Remote::WaitForClient()
{
    struct sockaddr_in serv_addr;

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd < 0)
        return false;
    memset((char *) &serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(10122);
    if (bind(serverfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        return false;

    listen(serverfd,1);
    clientfd = accept(serverfd, 0, 0);
    connected = clientfd >= 0;
    return connected;
}

void Remote::SendOK(int len)
{
    unsigned int size = htonl(len);
    unsigned short status = 0;
    send(clientfd,(char*)&size,4,0);
    send(clientfd,(char*)&status,2,0);
}

void Remote::SendFail()
{
    unsigned int size = htonl(2);
    unsigned short status = 1;
    send(clientfd,(char*)&size,4,0);
    send(clientfd,(char*)&status,2,0);
}

void Remote::Lock(char *data)
{
    SendOK();
}

void Remote::Unlock()
{
    SendOK();
}

void Remote::Command(char* data)
{
    if(!strcmp("SPISLOW",data))
    {
        clock = 20;
        programmer->SetTransferSpeed(clock);
    }

    SendOK();
}

void Remote::GetVar(char* data)
{
    string val;

    if(!strcmp("SPIMAXCLOCK",data))
        val = "1000";
    else if(!strcmp("SPICLOCK",data))
        val = static_cast<ostringstream*>(&(ostringstream() << clock))->str();
    else //unknown variable
    {
        cout << "GetVar - UNKNOWN: " << data << endl;
        SendFail();
        return;
    }

    unsigned int length = val.length() + 3;

    SendOK(length);
    send(clientfd,val.c_str(),length-2,0);
}

int Remote::SequenceSetVar(char* data, int len)
{
    string var, value;
    int olen = len;
    unsigned int nValue;

    while(*data >= 'A' && *data <= 'Z' && len > 0)
    {
        var += *data++;
        --len;
    }

    if(len > 0 && *data == 0)
    {
        data++;
        --len;
    }

    while(*data >= '0' && *data <= '9' && len > 0)
    {
        value += *data++;
        --len;
    }

    if(len > 0 && *data == 0)
        --len;

    nValue = strtol(value.c_str(),NULL,10);

    if(!var.compare("SPICLOCK"))
    {
        clock = nValue;
        programmer->SetTransferSpeed(clock);
    }

    return olen - len;
}

void Remote::Sequence(char* data, int len)
{
    unsigned short *buf = (unsigned short*)data;
    struct retdata *tmp = NULL;
    list<struct retdata*> ret;
    list<struct retdata*>::iterator it;
    bool failed = false;
    int resplen = 2;

    int ptr = 1;
    len -=2;

    for(int i=0;i<buf[0] && !failed;i++) {
        len -=2;
        switch(buf[ptr++]) {
        case 0:

            if(len < 4)
            {
                failed = true;
                break;
            }

            tmp = new struct retdata;
            tmp->len = buf[ptr+1];
            tmp->data = new unsigned short[tmp->len];
            ret.push_back(tmp);

            cout << "Sequence - Read: 0x" << hex << buf[ptr] << " 0x" << buf[ptr+1] << dec << endl;

            if(!programmer->ReadBlock(buf[ptr],buf[ptr+1],tmp->data))
            {
                failed = true;
                break;
            }

            resplen += tmp->len*2;
            ptr += 2;
            len -= 4;
            break;

        case 1:

            if(len < 6 || len < buf[ptr+1]*2)
            {
                failed = true;
                break;
            }

            cout << "Sequence - Write: 0x" << hex << buf[ptr] << " 0x" << buf[ptr+1] << dec << endl;

            if(!programmer->WriteBlock(buf[ptr],buf[ptr+1],&buf[ptr+2]))
            {
                failed = true;
                break;
            }

            len -= 4+buf[ptr+1]*2;
            ptr += 2+buf[ptr+1];
            break;
        case 2:
            int p = SequenceSetVar(&data[ptr*2],len);
            len -= p;
            ptr += p/2;
            break;
        }
    }

    if(!failed)
    {
        SendOK(resplen);

        for (it=ret.begin(); it != ret.end(); ++it)
            send(clientfd,(char*)(*it)->data,(*it)->len*2,0);
    } else
        SendFail();

    for (it=ret.begin(); it != ret.end(); ++it)
    {
        delete (*it)->data;
        delete (*it);
    }
}

void Remote::SetCoreType(char* data)
{
    coretype = *(unsigned short*)data;
    SendOK();
}

void Remote::GetCoreType()
{
    SendOK(4);
    send(clientfd,(char*)&coretype,2,0);
}

void Remote::IsXapStopped()
{
    cout << "IsXapStopped" << endl;
    unsigned short stopped = programmer->IsXAPStopped();
    SendOK(4);
    send(clientfd,(char*)&stopped,2,0);
}

void Remote::ResetAndStop()
{
    cout << "ResetAndStop" << endl;
    if(manager.XapResetAndStop())
        SendOK();
    else
        SendFail();
}

void Remote::Stop()
{
    cout << "Stop" << endl;
    if(manager.XapStop())
        SendOK();
    else
        SendFail();
}

void Remote::Go()
{
    cout << "Go" << endl;
    if(manager.XapGo())
        SendOK();
    else
        SendFail();
}

void Remote::ResetAndGo()
{
    cout << "ResetAndGo" << endl;
    if(manager.XapResetAndGo())
        SendOK();
    else
        SendFail();
}

void Remote::GetProcType()
{
    SendOK(4);
    send(clientfd,(char*)&proctype,2,0);
}

void Remote::SetProcType(char *data)
{
    proctype = *(unsigned short*)data;
    cout << "Proc type: " << proctype << endl;
    SendOK();
}

void Remote::Run()
{
    if(!programmer || !programmer->IsConnected()) {
        cout << "Programmer is not set or initialized!" << endl;
        return;
    }

    unsigned int datalen;
    char cmd[5];
    char *data;
    cmd[4] = 0;

    if(!connected) return;

    while(1)
    {
        if(!ReceiveData((char*)&datalen,4))
            break;
        datalen = ntohl(datalen);

        if(!ReceiveData(cmd,4))
            break;
        datalen -= 4;

        data = new char[datalen];
        if(!ReceiveData(data,datalen))
        {
            delete data;
            break;
        }

        if(!strcmp("lock",cmd))
            Lock(data);
        else if(!strcmp("getv",cmd))
            GetVar(data);
        else if(!strcmp("sequ",cmd))
            Sequence(data,datalen);
        else if(!strcmp("unlk",cmd))
            Unlock();
        else if(!strcmp("cmnd",cmd))
            Command(data);
        else if(!strcmp("stct",cmd))
            SetCoreType(data);
        else if(!strcmp("stpd",cmd))
            IsXapStopped();
        else if(!strcmp("gtct",cmd))
            GetCoreType();
        else if(!strcmp("rsst",cmd))
            ResetAndStop();
        else if(!strcmp("stop",cmd))
            Stop();
        else if(!strcmp("go  ",cmd))
            Go();
        else if(!strcmp("rsgo",cmd))
            ResetAndGo();
        else if(!strcmp("gtpt",cmd))
            GetProcType();
        else if(!strcmp("stpt",cmd))
            SetProcType(data);
        else if(!strcmp("ver ",cmd))
            SendOK();
        else
        {
            cout << "Unknown command: " << cmd << endl;
            cout << "Skipping " << datalen << " bytes..." << endl;
            SendFail();
        }

        delete data;
    }
}

void Remote::Disconnect()
{
    /*if(clientfd >= 0)
        closesocket(clientfd);
    if(serverfd >= 0)
        closesocket(serverfd);*/
    if(clientfd >= 0)
        close(clientfd);
    if(serverfd >= 0)
        close(serverfd);
}

Remote::~Remote()
{
    Disconnect();
    //WSACleanup();
}
