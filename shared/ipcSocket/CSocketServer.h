#ifndef C_SOCKET_SERVER_H
#define C_SOCKET_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

class CSocketServer
{
private:

    struct SClientData
    {
        int mNewSockFd;
        int mClientLen;
        struct sockaddr_in mClientAddr;
    };
    int mPort = 0;
    int mServerSockFd = 0;
    SClientData mClientData = {};

protected:

    bool mIsError = false;
    bool mIsClientConnected = false;
public:

    enum class RecvStatus
    {
        Ok = 1,
        Timeout = 0,
        Closed = -1,
        Error = -2
    };

    CSocketServer(int aPort = 9000);
    ~CSocketServer();
    void waitForClient();
    bool pollSocket();
    void closeSocket();
    RecvStatus receiveN(uint8_t* aBuffer, int aN, int aTimeoutMs = -1);
};

#endif // SOCKET_SERVER_H