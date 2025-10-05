#ifndef C_CALLBACK_INTERFACE_H
#define C_CALLBACK_INTERFACE_H

#include "lvgl/lvgl.h"

class CCallbackInterface
{
public:
    CCallbackInterface() = default;
    ~CCallbackInterface() = default;

    virtual void callback(lv_event_t * aEvent, uint32_t aId);
};

#endif // C_CALLBACK_INTERFACE_H
