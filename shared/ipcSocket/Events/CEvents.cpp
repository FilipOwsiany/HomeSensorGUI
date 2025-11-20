#include "CEvents.h"

// =============== TEMPERATURE =================
CEventTemperature::CEventTemperature() :
    CEventBase(EEventType::Temperature,
               sizeof(mTemperature),
               reinterpret_cast<uint8_t*>(&mTemperature))
{
}

CEventTemperature::~CEventTemperature() {}

bool CEventTemperature::serialize(CStream& aStream)
{
    aStream << mTemperature;
    return true;
}

bool CEventTemperature::unserialize(CStream& aStream)
{
    aStream >> mTemperature;
    return true;
}

// =============== PRESSURE =================
CEventPressure::CEventPressure() :
    CEventBase(EEventType::Pressure,
               sizeof(mPressure),
               reinterpret_cast<uint8_t*>(&mPressure))
{
}

CEventPressure::~CEventPressure() {}

bool CEventPressure::serialize(CStream& aStream)
{
    aStream << mPressure;
    return true;
}

bool CEventPressure::unserialize(CStream& aStream)
{
    aStream >> mPressure;
    return true;
}

// =============== HUMIDITY =================
CEventHumidity::CEventHumidity() :
    CEventBase(EEventType::Humidity,
               sizeof(mHumidity),
               reinterpret_cast<uint8_t*>(&mHumidity))
{
}

CEventHumidity::~CEventHumidity() {}

bool CEventHumidity::serialize(CStream& aStream)
{
    aStream << mHumidity;
    return true;
}

bool CEventHumidity::unserialize(CStream& aStream)
{
    aStream >> mHumidity;
    return true;
}

// =============== VOLTAGE =================
CEventVoltage::CEventVoltage() :
    CEventBase(EEventType::Voltage,
               sizeof(mVoltage),
               reinterpret_cast<uint8_t*>(&mVoltage))
{
}

CEventVoltage::~CEventVoltage() {}

bool CEventVoltage::serialize(CStream& aStream)
{
    aStream << mVoltage;
    return true;
}

bool CEventVoltage::unserialize(CStream& aStream)
{
    aStream >> mVoltage;
    return true;
}