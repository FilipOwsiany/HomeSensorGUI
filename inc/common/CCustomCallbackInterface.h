#ifndef C_CUSTOM_CALLBACK_INTERFACE_H
#define C_CUSTOM_CALLBACK_INTERFACE_H

#include "CCustomEventBase.h"

class CCustomCallbackInterface
{
public:
    CCustomCallbackInterface() = default;
    ~CCustomCallbackInterface() = default;

    virtual void callback(CCustomEventBase& aEvent) = 0;
};

#endif // C_CUSTOM_CALLBACK_INTERFACE_H
