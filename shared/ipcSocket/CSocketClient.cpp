#include "CSocketClient.h"
#include <unistd.h>
#include <poll.h>

#include <cstring>

CSocketClient::CSocketClient(int aPort) : mPort(aPort)
{
    std::memset(&mServerData, 0, sizeof(SServerData));
    mServerData.mAddr.sin_family = AF_INET;
    mServerData.mAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    mServerData.mAddr.sin_port = htons(aPort);

    connectToServer();
}

CSocketClient::~CSocketClient()
{
    std::cout << "Closing client socket\n";
    if (mClientSockFd > 0) 
    {
        close(mClientSockFd);
    }
}

CSocketClient::SocketClientStatus CSocketClient::sendN(const uint8_t* aBuffer, int aN)
{
    if (!mIsConnected || mIsError)
    {
        return SocketClientStatus::Disconnected;
    }

    size_t sent = 0;
    const uint8_t* p = aBuffer;

    while (sent < static_cast<size_t>(aN))
    {
        ssize_t n = send(mClientSockFd, p + sent, aN - sent, 0);
        std::cout << "Sending data: ";
        for(size_t i = 0; i < static_cast<size_t>(aN); i++)
        {
            std::cout << "0x" 
                << std::hex << std::uppercase << static_cast<int>(aBuffer[i]) 
                << std::dec << " ";
        }
        std::cout << "\n";

        if (n > 0)
        {
            sent += n;
            continue;
        }

        if (n == 0)
        {
            mIsConnected = false;
            close(mClientSockFd);
            mClientSockFd = -1;
            return SocketClientStatus::Disconnected;
        }

        if (errno == EPIPE || errno == ECONNRESET)
        {
            mIsConnected = false;
            close(mClientSockFd);
            mClientSockFd = -1;
            return SocketClientStatus::Disconnected;
        }

        perror("send");
        mIsError = true;
        return SocketClientStatus::Error;
    }

    return SocketClientStatus::Ok;
}

CSocketClient::SocketClientStatus CSocketClient::connectToServer()
{
    if (mIsConnected)
    {
        return SocketClientStatus::Ok;
    }

    if (mClientSockFd < 0)
    {
        mClientSockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (mClientSockFd < 0)
        {
            perror("socket");
            mIsError = true;
            return SocketClientStatus::Error;
        }
    }

    if (connect(mClientSockFd,
                (struct sockaddr*)&mServerData.mAddr,
                sizeof(mServerData.mAddr)) < 0)
    {
        perror("connect");

        close(mClientSockFd);
        mClientSockFd = -1;
        mIsConnected = false;
        mIsError = true;

        // odróżniamy błąd vs brak serwera
        if (errno == ECONNREFUSED || errno == ENETUNREACH || errno == ETIMEDOUT)
        {
            return SocketClientStatus::Disconnected; // serwer nieosiągalny
        }

        return SocketClientStatus::Error; // inny błąd
    }

    mIsConnected = true;
    mIsError = false;
    return SocketClientStatus::Ok;
}