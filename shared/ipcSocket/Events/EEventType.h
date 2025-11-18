#ifndef E_EVENT_TYPE_H
#define E_EVENT_TYPE_H

enum class EEventType : int
{
    None = 0,
    Temperature = 1,
    Humidity = 2,
    Pressure = 3,
    Voltage = 4
};

#endif // E_EVENT_TYPE_H