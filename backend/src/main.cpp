#include <iostream>
#include <vector>
#include <string>

#include "CSender.h"
#include "CEvents.h"
#include "CBackend.h"
#include "CLogger.h"

int main()
{
    CLogger::instance().setLevel(CLogger::Level::DEBUG);
    CLogger::instance().setMaxQueueSize(5000);
    CLogger::instance().setMaxLogSize(256);
    CLogger::instance().enableFile("/tmp/backend.log");

    LOG_INFO("Backend application started");
    const std::string host = "home.meryyfifonsz.xyz";
    CBackend backend(host);

    if (!backend.isValid())
    {
        return 1;
    }

    if (!backend.readEnv())
    {
        return 1;
    }
    
    if (!backend.authorize())
    {
        return 1;
    }

    CSender sender(9000);
    
    while (true)
    {
        auto result = backend.getSensorMeasurments();

        if (!result) 
        {
            continue;
        }

        SDataContainerSensorMeasurment mean{};
        uint16_t sampleNumber = 0;

        for (auto& s : *result)
        {
            LOGF_DEBUG("ts: %llu  mTemperature: %f  mPressure: %f  mHumidity: %f  mVoltage: %u",
                      s.timestamp,
                      s.mTemperature,
                      s.mPressure,
                      s.mHumidity,
                      s.mVoltage);

            mean.mTemperature   += s.mTemperature;
            mean.mPressure      += s.mPressure;
            mean.mHumidity      += s.mHumidity;
            mean.mVoltage       += s.mVoltage;
            sampleNumber++;
        }

        if (sampleNumber == 0)
        {
            continue;
        }

        mean.mTemperature   /= sampleNumber;
        mean.mPressure      /= sampleNumber;
        mean.mHumidity      /= sampleNumber;
        mean.mVoltage       /= sampleNumber;

        constexpr double SCALE = 1000.0;

        uint32_t temperatureScaled      = static_cast<uint32_t>(mean.mTemperature * SCALE);
        uint32_t pressureScaled         = static_cast<uint32_t>(mean.mPressure    * SCALE);
        uint32_t humidityScaled         = static_cast<uint32_t>(mean.mHumidity    * SCALE);
        uint32_t voltageScaled          = static_cast<uint32_t>(mean.mVoltage     * SCALE);

        // ----- Temperature -----
        CEventTemperature* temperature = new CEventTemperature();
        temperature->setTemperature(temperatureScaled);
        sender.sendEvent(temperature);

        // ----- Pressure -----
        CEventPressure* pressure = new CEventPressure();
        pressure->setPressure(pressureScaled);
        sender.sendEvent(pressure);

        // ----- Humidity -----
        CEventHumidity* humidity = new CEventHumidity();
        humidity->setHumidity(humidityScaled);
        sender.sendEvent(humidity);

        // ----- Voltage -----
        CEventVoltage* voltage = new CEventVoltage();
        voltage->setVoltage(voltageScaled);
        sender.sendEvent(voltage);

        sleep(10);
    }

    return 0;
}
