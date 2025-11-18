#ifndef C_SOCKET_CLIENT_H
#define C_SOCKET_CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

class CSocketClient
{
private:

    struct SServerData
    {
        int mNewSockFd;
        int mLen;
        struct sockaddr_in mAddr;
    };
    int mPort = 0;
    int mClientSockFd = -1;
    SServerData mServerData = {};
protected:

    bool mIsError = false;
    bool mIsConnected = false;
public:

    enum class SocketClientStatus
    {
        Ok,
        Error,
        Disconnected
    };

    CSocketClient(int aPort = 9000);
    ~CSocketClient();
    SocketClientStatus sendN(const uint8_t* aBuffer, int aN);
    SocketClientStatus connectToServer();
};

#endif // C_SOCKET_CLIENT_H