#include "CEventBase.h"

CEventBase::CEventBase(EEventType aEventType, int aPayloadSize, uint8_t* aPayload, uint16_t aPayloadExpectedSize) 
    : mEventType(aEventType), 
      mPayloadMaxSize(aPayloadSize), 
      mPayload(aPayload),
      mPayloadCurrentSize(aPayloadExpectedSize)
{
}

CEventBase::~CEventBase()
{
}