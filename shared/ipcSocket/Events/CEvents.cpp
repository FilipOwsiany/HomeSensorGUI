#include "CEvents.h"

CEventTemperature::CEventTemperature() :
    CEventBase(EEventType::Temperature, sizeof(mTemperature), reinterpret_cast<uint8_t*>(&mTemperature))
{
}

CEventTemperature::~CEventTemperature()
{
}

bool CEventTemperature::serialize(CStream& aStream)
{
    std::cout << "CEventTemperature::serialize(CStream& aStream)" << std::endl;
    aStream << mTemperature;
    return true;
}

bool CEventTemperature::unserialize(CStream& aStream)
{
    aStream >> mTemperature;
    return true;
}