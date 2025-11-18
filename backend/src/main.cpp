#include <iostream>
#include <vector>
#include <string>

#include "CSender.h"
#include "CEvents.h"
#include "CBackend.h"

int main()
{
    const std::string host = "home.meryyfifonsz.xyz";
    CBackend backend = CBackend(host);

    if (false == backend.isValid())
    {
        return 1;
    }
    
    if (false == backend.authorize())
    {
        return 1;
    }

    CSender sender = CSender(9000);
    
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
                    << "  mTemperature: "   << s.mTemperature
                    << "  mPressure: "      << s.mPressure
                    << "  mHumidity: "      << s.mHumidity
                    << "  mVoltage: "       << s.mVoltage
                    << "\n";

            mean.mTemperature   += s.mTemperature;
            mean.mPressure      += s.mPressure;
            mean.mHumidity      += s.mHumidity;
            mean.mVoltage       += s.mVoltage;
            sampleNumber++;
        }

        mean.mTemperature   /= sampleNumber;
        mean.mPressure      /= sampleNumber;
        mean.mHumidity      /= sampleNumber;
        mean.mVoltage       /= sampleNumber;

        printf("mean.mTemperature %f\n", mean.mTemperature);
        printf("mean.mTemperature %d\n", static_cast<uint32_t>((mean.mTemperature * 1000.0) / 1000));

        CEventTemperature* temperature = new CEventTemperature();
        temperature->setTemperature(static_cast<uint32_t>(mean.mTemperature * 1000.0));
        sender.sendEvent(temperature);
        sleep(10);
    }

    return 0;
}