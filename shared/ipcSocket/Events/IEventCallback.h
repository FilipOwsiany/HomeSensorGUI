#ifndef I_EVENT_CALLBACK
#define I_EVENT_CALLBACK

#include "CEventBase.h"

class IEventCallback
{
public:
    virtual ~IEventCallback() = default;
    virtual void onEvent(CEventBase* aEvent) = 0;
};
#endif // I_EVENT_CALLBACK