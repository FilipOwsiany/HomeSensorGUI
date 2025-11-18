#ifndef C_RECEIVER_H
#define C_RECEIVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

#include "CSocketServer.h"

class CReceiver : public CSocketServer
{
private:

    pthread_t mReceiverThread = 0;

    static void* receiver(void* arg);
    void waitForClient();
    void handleReviceStatus(RecvStatus status);
public:

    CReceiver(/* args */);
    ~CReceiver();
};

#endif // C_RECEIVER_H