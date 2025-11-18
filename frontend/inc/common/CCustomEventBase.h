#ifndef C_CUSTOM_EVENT_BASE_H
#define C_CUSTOM_EVENT_BASE_H

#include "lvgl/lvgl.h"

class CCustomEventBase
{
public:

    enum class EEventId : uint32_t
    {
        NONE = 0,
        WIFI,
        CLOUD,
        BLUETOOTH,
        NAME,
        TEMPERATURE,
        HUMIDITY,
        PRESSURE,
        BATTERY
    };
private:
    EEventId mId;
public:
    CCustomEventBase() = default;
    ~CCustomEventBase() = default;

    EEventId getId() const { return mId; }
    void setId(EEventId aId) { mId = aId; }
};

#endif // C_CUSTOM_EVENT_BASE_H