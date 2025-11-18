#include "CSocketServer.h"
#include <unistd.h>
#include <poll.h>

#include <cstring>

CSocketServer::CSocketServer(int aPort) : mPort(aPort)
{
    std::cout << "Starting server on port " << mPort << "\n";
    mServerSockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mServerSockFd < 0)
    {
        std::cerr << "Failed to create socket\n";
        mIsError = true;
        return;
    }

    int optval = 1;
    if (setsockopt(mServerSockFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) 
    {
        std::cerr << "Failed to set socket options\n";
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
        std::cerr << "Failed to bind socket\n";
        mIsError = true;
        return;
    }

    listen(mServerSockFd, 1);
}


CSocketServer::~CSocketServer()
{
    std::cout << "Closing server socket\n";
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

    std::cout << "Client connected: " 
              << inet_ntoa(mClientData.mClientAddr.sin_addr) 
              << ":" << ntohs(mClientData.mClientAddr.sin_port) << "\n";
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
        std::cerr << "Poll error\n";
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
        std::cout << "Receiving data: ";
        for(size_t i = 0; i < static_cast<size_t>(aN); i++)
        {
            std::cout << "0x" 
                << std::hex << std::uppercase << static_cast<int>(aBuffer[i]) 
                << std::dec << " ";
        }
        std::cout << "\n";

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

