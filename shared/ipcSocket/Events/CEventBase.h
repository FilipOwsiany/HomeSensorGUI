#ifndef C_EVENT_BASE_H
#define C_EVENT_BASE_H

#include "stdint.h"
#include <cstring>

#include "EEventType.h"
#include "CStream.h"

class CEventBase
{
protected:
    EEventType mEventType = EEventType::None;
    uint16_t mPayloadMaxSize = 0;
    uint16_t mPayloadCurrentSize = 0;
    uint8_t* mPayload = nullptr;
public:
    CEventBase(EEventType aEventType, int aPayloadSize, uint8_t* aPayload, uint16_t aPayloadExpectedSize = 0);
    ~CEventBase();

    EEventType getEventType() const { return mEventType; }
    int getPayloadMaxSize() const { return mPayloadMaxSize; }
    uint8_t* getPayload() const { return mPayload; }
    bool getIsPayloadOk() const { return mPayloadCurrentSize <= mPayloadMaxSize; }

    void setPayloadCurrentSize(uint16_t aSize) { mPayloadCurrentSize = aSize; }

    virtual bool serialize(CStream& aStream) = 0;
    virtual bool unserialize(CStream& aStream) = 0;
};

#endif // C_EVENT_BASE_H