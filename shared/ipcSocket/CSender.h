#ifndef C_SENDER_H
#define C_SENDER_H

#include "CSocketClient.h"
#include "CEventBase.h"

class CSender : public CSocketClient
{
private:
    /* data */
public:
    CSender(int aPort);
    ~CSender();
    bool sendEvent(CEventBase* aEvent);
};

#endif // C_SENDER_H