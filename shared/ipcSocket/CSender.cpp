#include "CSender.h"
#include "CStream.h"

CSender::CSender(int aPort) : CSocketClient(aPort)
{
}

CSender::~CSender()
{
}

bool CSender::sendEvent(CEventBase* aEvent)
{
    uint16_t bufferSize = aEvent->getPayloadMaxSize();
    uint8_t buffer[bufferSize] = {0};
    uint16_t headerSize = sizeof(uint8_t) + sizeof(uint16_t);
    uint8_t header[headerSize] = {0};

    CStream stream = CStream(buffer, bufferSize, CStream::EDirection::IN);

    aEvent->serialize(stream);

    uint16_t payloadSize = stream.getSize();
    header[0] = static_cast<uint8_t>(aEvent->getEventType());
    header[1] = static_cast<uint8_t>(payloadSize & 0xFF);
    header[2] = static_cast<uint8_t>((payloadSize >> 8) & 0xFF);

    SocketClientStatus status = sendN(header, headerSize);   
    status = sendN(buffer, payloadSize);

    delete aEvent;

    return true;
}
