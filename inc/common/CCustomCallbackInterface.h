#ifndef C_CUSTOM_CALLBACK_INTERFACE_H
#define C_CUSTOM_CALLBACK_INTERFACE_H

#include "CEventBase.h"

class CCustomCallbackInterface
{
public:
    CCustomCallbackInterface() = default;
    ~CCustomCallbackInterface() = default;

    virtual void callback(CEventBase& aEvent) = 0;
};

#endif // C_CUSTOM_CALLBACK_INTERFACE_H
