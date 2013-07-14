#ifndef REMOTE_H
#define REMOTE_H

//#include <Windows.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "devicemanager.h"
#include "programmer.h"

class Remote
{
private:
    int serverfd, clientfd;
    bool connected;
	unsigned int clock;
    unsigned short coretype,proctype;
    Programmer *programmer;
    DeviceManager manager;
private:
    bool ReceiveData(char* buf, int len);
	void SendOK(int len=2);
	void SendFail();
    void Lock(char* data);
    void Unlock();
    void GetVar(char* data);
	void Command(char* data);
	int SequenceSetVar(char* data, int len);
    void Sequence(char* data, int len);
	void SetCoreType(char* data);
	void GetCoreType();
    void SetProcType(char* data);
    void GetProcType();
	void IsXapStopped();
	void ResetAndStop();
	void Stop();
	void Go();
    void ResetAndGo();
public:
    Remote(Programmer *prog);
    ~Remote();
    bool WaitForClient();
    void Disconnect();
    void Run();
};

#endif // REMOTE_H
