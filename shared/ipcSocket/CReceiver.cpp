#include "CReceiver.h"

#include "CStream.h"
#include "EEventType.h"
#include "CEventBase.h"
#include "CEventFactory.h"
#include "CEventDispatcher.h"


CReceiver::CReceiver(/* args */) : CSocketServer(9000)
{
    waitForClient();

    std::cout << "Starting receiver thread in CReceiver\n";

    pthread_create(&mReceiverThread, nullptr, 
                   (void* (*)(void*)) CReceiver::receiver, 
                   static_cast<void*>(this));   
}

CReceiver::~CReceiver()
{

}

void* CReceiver::receiver(void* arg)
{
    std::cout << "Receiver thread started\n";
    CReceiver* receiver = static_cast<CReceiver*>(arg);

    while (true)
    {
        uint8_t evnetType = 0;
        RecvStatus status = receiver->receiveN(&evnetType, 1, -1);
        if (status == RecvStatus::Ok)
        {
            std::cout << "Received data: 0x" 
                << std::hex << std::uppercase << static_cast<int>(evnetType) 
                << std::dec << "\n";
        }
        else if (status == RecvStatus::Timeout)
        {
            std::cout << "Receive timeout\n";
            continue;
        }
        else if (status == RecvStatus::Closed)
        {
            std::cout << "Client disconnected, waiting for new client...\n";
            receiver->waitForClient();
            continue;
        }
        else
        {
            std::cerr << "Receive error\n";
            continue;
        }   

        uint16_t evnetLen = 0;
        status = receiver->receiveN(reinterpret_cast<uint8_t*>(&evnetLen), 2, -1);
        if (status == RecvStatus::Ok)
        {
            std::cout << "Received data: 0x" 
                << std::hex << std::uppercase << static_cast<int>(evnetLen) 
                << std::dec << "\n";
        }
        else if (status == RecvStatus::Timeout)
        {
            std::cout << "Receive timeout\n";
            continue;
        }
        else if (status == RecvStatus::Closed)
        {
            std::cout << "Client disconnected, waiting for new client...\n";
            receiver->waitForClient();
            continue;
        }
        else
        {
            std::cerr << "Receive error\n";
            continue;
        }   

        CEventBase* event = CEventFactory::getInstance().createEvent(static_cast<EEventType>(evnetType));
        
        if (event != nullptr)
        {
            uint8_t buffer[evnetLen] = {0};
            status = receiver->receiveN(buffer, evnetLen, -1);
            
            for (size_t i = 0; i < evnetLen; i++)
            {
                std::cout << "Payload data[" << i << "]: " << std::hex << static_cast<int>(buffer[i])  << "\n";
            }
            CStream stream(buffer, evnetLen, CStream::EDirection::OUT);
            event->unserialize(stream);

            
            CEventDispatcher::getInstance().dispatchEvent(event);

            delete event;
        }
        
    }

    return nullptr;
}

void CReceiver::waitForClient()
{
    while (mIsClientConnected == false) 
    {
        CSocketServer::waitForClient();
    }
}

void CReceiver::handleReviceStatus(RecvStatus status)
{
    if (status == RecvStatus::Ok)
    {
        std::cout << "Received data!\n";
    }
    else if (status == RecvStatus::Timeout)
    {
        std::cout << "Receive timeout\n";
    }
    else if (status == RecvStatus::Closed)
    {
        std::cout << "Client disconnected, waiting for new client...\n";
        waitForClient();
    }
    else
    {
        std::cerr << "Receive error\n";
    } 
}