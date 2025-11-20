#ifndef C_EVENTS_H
#define C_EVENTS_H

#include "CEventBase.h"

class CEventTemperature : public CEventBase
{
private:
    uint32_t mTemperature = 0;
public:
    CEventTemperature();
    ~CEventTemperature();
    uint32_t getTemperature() const { return mTemperature; }
    void setTemperature(uint32_t aTemperature) { mTemperature = aTemperature; }

    bool serialize(CStream& aStream) override;
    bool unserialize(CStream& aStream) override;
};

class CEventPressure : public CEventBase
{
private:
    uint32_t mPressure = 0;
public:
    CEventPressure();
    ~CEventPressure();
    uint32_t getPressure() const { return mPressure; }
    void setPressure(uint32_t aPressure) { mPressure = aPressure; }

    bool serialize(CStream& aStream) override;
    bool unserialize(CStream& aStream) override;
};

class CEventHumidity : public CEventBase
{
private:
    uint32_t mHumidity = 0;
public:
    CEventHumidity();
    ~CEventHumidity();
    uint32_t getHumidity() const { return mHumidity; }
    void setHumidity(uint32_t aHumidity) { mHumidity = aHumidity; }

    bool serialize(CStream& aStream) override;
    bool unserialize(CStream& aStream) override;
};

class CEventVoltage : public CEventBase
{
private:
    uint32_t mVoltage = 0;
public:
    CEventVoltage();
    ~CEventVoltage();
    uint32_t getVoltage() const { return mVoltage; }
    void setVoltage(uint32_t aVoltage) { mVoltage = aVoltage; }

    bool serialize(CStream& aStream) override;
    bool unserialize(CStream& aStream) override;
};

#endif // C_EVENTS_H