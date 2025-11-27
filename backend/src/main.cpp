#include <iostream>
#include <vector>
#include <string>

#include "CSender.h"
#include "CEvents.h"
#include "CBackend.h"

int main()
{
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
            std::cout << "ts: "             << s.timestamp
                      << "  mTemperature: " << s.mTemperature
                      << "  mPressure: "    << s.mPressure
                      << "  mHumidity: "    << s.mHumidity
                      << "  mVoltage: "     << s.mVoltage
                      << "\n";

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

        printf("mean.mTemperature %f\n", mean.mTemperature);
        printf("mean.mTemperature %u\n",
            static_cast<uint32_t>((mean.mTemperature * 1000.0) / 1000.0));

        printf("mean.mPressure %f\n", mean.mPressure);
        printf("mean.mPressure %u\n",
            static_cast<uint32_t>((mean.mPressure * 1000.0) / 1000.0));

        printf("mean.mHumidity %f\n", mean.mHumidity);
        printf("mean.mHumidity %u\n",
            static_cast<uint32_t>((mean.mHumidity * 1000.0) / 1000.0));

        printf("mean.mVoltage %f\n", mean.mVoltage);
        printf("mean.mVoltage %u\n",
            static_cast<uint32_t>((mean.mVoltage * 1000.0) / 1000.0));


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
