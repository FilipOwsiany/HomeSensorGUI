#include "CSocketServer.h"
#include "CLogger.h"
#include <unistd.h>
#include <poll.h>

#include <cstring>

CSocketServer::CSocketServer(int aPort) : mPort(aPort)
{
    LOGF_DEBUG("Starting server on port %d", mPort);
    mServerSockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mServerSockFd < 0)
    {
        LOG_ERROR("Failed to create socket");
        mIsError = true;
        return;
    }

    int optval = 1;
    if (setsockopt(mServerSockFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) 
    {
        LOG_ERROR("Failed to set socket options");
        mIsError = true;
        return;
    }

    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;  
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    serverAddr.sin_port = htons(mPort);  

    if (bind(mServerSockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        LOG_ERROR("Failed to bind socket");
        mIsError = true;
        return;
    }

    listen(mServerSockFd, 1);
}


CSocketServer::~CSocketServer()
{
    LOG_DEBUG("Closing server socket");
    if (mServerSockFd > 0) {
        close(mServerSockFd);
    }
}

void CSocketServer::waitForClient()
{
    std::memset(&mClientData, 0, sizeof(SClientData));
    mClientData.mClientLen = sizeof(struct sockaddr_in);
    mClientData.mNewSockFd = accept(mServerSockFd, 
                                    (struct sockaddr *) &mClientData.mClientAddr, 
                                    (socklen_t *) &mClientData.mClientLen);

    mIsClientConnected = true;

    LOGF_INFO("Client connected: %s:%d", 
              inet_ntoa(mClientData.mClientAddr.sin_addr), 
              ntohs(mClientData.mClientAddr.sin_port));
}

bool CSocketServer::pollSocket()
{
    struct pollfd pfd = 
    {
        .fd = mClientData.mNewSockFd,
        .events = POLLIN,
    };

    int ret = poll(&pfd, 1, -1);

    if (ret < 0) 
    {
        LOG_ERROR("Poll error on socket");
        return false;
    }

    if (pfd.revents & POLLIN) 
    {
        return true;
    }

    return false;
}

void CSocketServer::closeSocket()
{
    if (mClientData.mNewSockFd > 0) 
    {
        close(mClientData.mNewSockFd);
    }

    mIsClientConnected = false;
    std::memset(&mClientData, 0, sizeof(SClientData));
}

CSocketServer::RecvStatus CSocketServer::receiveN(uint8_t* aBuffer, int aN, int aTimeoutMs)
{
    uint8_t* p = aBuffer;
    size_t got = 0;

    while (got < aN) 
    {
        struct pollfd pfd = 
        {
            .fd = mClientData.mNewSockFd,
            .events = POLLIN,
        };
        int pr = poll(&pfd, 1, aTimeoutMs);
        if (pr < 0) 
        {
            if (errno == EINTR)
            {
                continue;
            } 
            return RecvStatus::Error;
        }
        if (pr == 0)
        {
            return RecvStatus::Timeout;
        }
        if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) 
        {
            return RecvStatus::Closed;
        }

        ssize_t r = recv(mClientData.mNewSockFd, (p + got), (aN - got), 0);
        LOG_DEBUG("Receiving data: ");
        for(size_t i = 0; i < static_cast<size_t>(aN); i++)
        {
            LOGF_DEBUG("%02X ", aBuffer[i]);
        }
        LOG_DEBUG("");

        if (r > 0) 
        { 
            got += (size_t)r; 
            continue; 
        }
        if (r == 0)  
        {
            mIsClientConnected = false;
            return RecvStatus::Closed;
        }
        if (errno == EINTR) 
        {
            continue;
        }
        if (errno == EAGAIN || errno == EWOULDBLOCK) 
        {
            continue;
        }
        return RecvStatus::Error;
    }
    return RecvStatus::Ok;
}

