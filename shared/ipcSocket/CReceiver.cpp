#include "CReceiver.h"
#include "CLogger.h"
#include "CStream.h"
#include "EEventType.h"
#include "CEventBase.h"
#include "CEventFactory.h"
#include "CEventDispatcher.h"


CReceiver::CReceiver(/* args */) : CSocketServer(9000)
{
    waitForClient();

    LOG_DEBUG("Starting receiver thread in CReceiver");

    pthread_create(&mReceiverThread, nullptr, 
                   (void* (*)(void*)) CReceiver::receiver, 
                   static_cast<void*>(this));   
}

CReceiver::~CReceiver()
{

}

void* CReceiver::receiver(void* arg)
{
    LOG_DEBUG("Receiver thread started");
    CReceiver* receiver = static_cast<CReceiver*>(arg);

    while (true)
    {
        uint8_t evnetType = 0;
        RecvStatus status = receiver->receiveN(&evnetType, 1, -1);
        if (status == RecvStatus::Ok)
        {
            LOGF_DEBUG("Received event type: 0x%02X", evnetType);
        }
        else if (status == RecvStatus::Timeout)
        {
            std::cout << "Receive timeout\n";
            continue;
        }
        else if (status == RecvStatus::Closed)
        {
            LOG_DEBUG("Client disconnected, waiting for new client...");
            receiver->waitForClient();
            continue;
        }
        else
        {
            LOG_DEBUG("Receive error");
            continue;
        }   

        uint16_t evnetLen = 0;
        status = receiver->receiveN(reinterpret_cast<uint8_t*>(&evnetLen), 2, -1);
        if (status == RecvStatus::Ok)
        {
            LOGF_DEBUG("Received len: 0x%04X", evnetLen);
        }
        else if (status == RecvStatus::Timeout)
        {
            LOG_WARN("Receive timeout");
            continue;
        }
        else if (status == RecvStatus::Closed)
        {
            LOG_WARN("Client disconnected, waiting for new client...");
            receiver->waitForClient();
            continue;
        }
        else
        {
            LOG_ERROR("Receive error");
            continue;
        }   

        CEventBase* event = CEventFactory::getInstance().createEvent(static_cast<EEventType>(evnetType));
        
        if (event != nullptr)
        {
            uint8_t buffer[evnetLen] = {0};
            status = receiver->receiveN(buffer, evnetLen, -1);
            
            for (size_t i = 0; i < evnetLen; i++)
            {
                LOGF_DEBUG("Payload data[%zu]: %02X", i, buffer[i]);
            }
            CStream stream(buffer, evnetLen, CStream::EDirection::OUT);
            event->unserialize(stream);

            
            CEventDispatcher::getInstance().dispatchEvent(event);

            delete event;
        }
        else
        {
            LOG_ERROR("Failed to create event");
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
        LOG_DEBUG("Received data successfully");
    }
    else if (status == RecvStatus::Timeout)
    {
        LOG_WARN("Receive timeout");
    }
    else if (status == RecvStatus::Closed)
    {
        LOG_WARN("Client disconnected, waiting for new client...");
        waitForClient();
    }
    else
    {
        LOG_ERROR("Receive error");
    } 
}