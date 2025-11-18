#ifndef C_LVGL_CALLBACK_INTERFACE_H
#define C_LVGL_CALLBACK_INTERFACE_H

#include "lvgl/lvgl.h"

class CLVGLCallbackInterface
{
public:
    CLVGLCallbackInterface() = default;
    ~CLVGLCallbackInterface() = default;

    virtual void callback(lv_event_t * aEvent, uint32_t aId);
};

#endif // C_LVGL_CALLBACK_INTERFACE_H
