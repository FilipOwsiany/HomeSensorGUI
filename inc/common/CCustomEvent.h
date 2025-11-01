#ifndef C_CUSTOM_EVENT_H
#define C_CUSTOM_EVENT_H

#include "CEventBase.h"

class CEventWifi : public CEventBase
{
public:
    enum class EEventWifiType
    {
        CONNECTED,
        DISCONNECTED,
        GOT_IP,
        LOST_IP
    };
private:
    EEventWifiType mWifiEventType;
public:
    CEventWifi()
    {
        setId(EEventId::WIFI);
    }
    ~CEventWifi() = default;    

    EEventWifiType getWifiEventType() const { return mWifiEventType; }
    void setWifiEventType(EEventWifiType aType) { mWifiEventType = aType; }
};

class CEventCloud : public CEventBase
{
public:
    enum class EEventCloudType
    {
        CONNECTED,
        DISCONNECTED,
        DATA_SENT,
        DATA_RECEIVED
    };
private:
    EEventCloudType mCloudEventType;
public:
    CEventCloud()
    {
        setId(EEventId::CLOUD);
    }
    ~CEventCloud() = default;    

    EEventCloudType getCloudEventType() const { return mCloudEventType; }
    void setCloudEventType(EEventCloudType aType) { mCloudEventType = aType; }
};

class CEventBluetooth : public CEventBase
{
public:
    enum class EEventBluetoothType
    {
        CONNECTED,
        DISCONNECTED,
        DATA_SENT,
        DATA_RECEIVED
    };
private:
    EEventBluetoothType mBluetoothEventType;
public:
    CEventBluetooth()
    {
        setId(EEventId::BLUETOOTH);
    }
    ~CEventBluetooth() = default;    

    EEventBluetoothType getBluetoothEventType() const { return mBluetoothEventType; }
    void setBluetoothEventType(EEventBluetoothType aType) { mBluetoothEventType = aType; }
};

class CEventSensorName : public CEventBase
{
public:
private:
    std::string mName;
    uint32_t mSensorId;
public:
    CEventSensorName()
    {
        setId(EEventId::NAME);
    }
    ~CEventSensorName() = default;    

    std::string getSensorName() const { return mName; }
    void setSensorName(std::string aName) { mName = aName; }

    uint32_t getSensorId() const { return mSensorId; }
    void setSensorId(uint32_t aSensorId) { mSensorId = aSensorId; }
};

class CEventSensorTemperature : public CEventBase
{
public:
private:
    uint32_t mTemperature;
    uint32_t mSensorId;
public:
    CEventSensorTemperature()
    {
        setId(EEventId::TEMPERATURE);
    }
    ~CEventSensorTemperature() = default;    

    uint32_t getSensorTemperature() const { return mTemperature; }
    void setSensorTemperature(uint32_t aTemperature) { mTemperature = aTemperature; }

    uint32_t getSensorId() const { return mSensorId; }
    void setSensorId(uint32_t aSensorId) { mSensorId = aSensorId; }
};

class CEventSensorHumidity : public CEventBase
{
public:
private:
    uint32_t mHumidity;
    uint32_t mSensorId;
public:
    CEventSensorHumidity()
    {
        setId(EEventId::HUMIDITY);
    }
    ~CEventSensorHumidity() = default;
    uint32_t getSensorHumidity() const { return mHumidity; }
    void setSensorHumidity(uint32_t aHumidity) { mHumidity = aHumidity; }
    uint32_t getSensorId() const { return mSensorId; }
    void setSensorId(uint32_t aSensorId) { mSensorId = aSensorId; }
};  

class CEventSensorPressure : public CEventBase
{
public:
private:
    uint32_t mPressure;
    uint32_t mSensorId;
public:
    CEventSensorPressure()
    {
        setId(EEventId::PRESSURE);
    }
    ~CEventSensorPressure() = default;    
    uint32_t getSensorPressure() const { return mPressure; }
    void setSensorPressure(uint32_t aPressure) { mPressure = aPressure; }
    uint32_t getSensorId() const { return mSensorId; }
    void setSensorId(uint32_t aSensorId) { mSensorId = aSensorId; }
};  

class CEventSensorBattery : public CEventBase
{
public:
private:
    uint32_t mBatteryLevel;
    uint32_t mSensorId;
public:
    CEventSensorBattery()
    {
        setId(EEventId::BATTERY);
    }
    ~CEventSensorBattery() = default;    
    uint32_t getSensorBatteryLevel() const { return mBatteryLevel; }
    void setSensorBatteryLevel(uint32_t aBatteryLevel) { mBatteryLevel = aBatteryLevel; }
    uint32_t getSensorId() const { return mSensorId; }
    void setSensorId(uint32_t aSensorId) { mSensorId = aSensorId; }
};  

#endif // C_CUSTOM_EVENT_H