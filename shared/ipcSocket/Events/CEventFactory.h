#ifndef C_EVENT_FACTORY_H
#define C_EVENT_FACTORY_H

#include "EEventType.h"
#include "CEventBase.h"
#include "CEvents.h"

class CEventFactory
{
private:

    CEventFactory() = default;
    ~CEventFactory() = default;
public:

    static CEventFactory& getInstance()
    {
        static CEventFactory instance;
        return instance;
    }

    CEventBase* createEvent(EEventType aEventType)
    {
        switch (aEventType)
        {
            case EEventType::Temperature:
                return new CEventTemperature();
            case EEventType::Humidity:
                return new CEventHumidity();
            case EEventType::Pressure:
                return new CEventPressure();
            case EEventType::Voltage:
                return new CEventVoltage();
            default:
                return nullptr;
        }
    }
};



#endif // C_EVENT_FACTORY_H


