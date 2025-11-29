#include "CSocketClient.h"
#include "CLogger.h"
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
    LOG_DEBUG("Closing client socket");
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
        LOG_DEBUG("Sending data: ");
        for(size_t i = 0; i < static_cast<size_t>(aN); i++)
        {
            LOGF_DEBUG("%02X ", aBuffer[i]);
        }
        LOGF_DEBUG("");

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

        LOG_ERROR("Error sending data");

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
            LOG_ERROR("Failed to create socket");
            mIsError = true;
            return SocketClientStatus::Error;
        }
    }

    if (connect(mClientSockFd,
                (struct sockaddr*)&mServerData.mAddr,
                sizeof(mServerData.mAddr)) < 0)
    {
        LOG_DEBUG("Failed to connect to server");

        close(mClientSockFd);
        mClientSockFd = -1;
        mIsConnected = false;
        mIsError = true;

        if (errno == ECONNREFUSED || errno == ENETUNREACH || errno == ETIMEDOUT)
        {
            return SocketClientStatus::Disconnected;
        }

        return SocketClientStatus::Error;
    }

    mIsConnected = true;
    mIsError = false;
    return SocketClientStatus::Ok;
}