#ifndef S_DATA_CONTAINER_H
#define S_DATA_CONTAINER_H

#include <string>
#include <optional>
#include <cstdint>

struct SDataContainerAuthorization
{
    std::string token;
    std::optional<std::string> refreshToken;
    std::optional<long long>   expiresAt;
};

struct SDataContainerSensorMeasurment
{
    uint64_t timestamp      = 0;
    float    mTemperature   = 0;
    float    mPressure      = 0;
    float    mHumidity      = 0;
    uint32_t mVoltage       = 0;
};

#endif // S_DATA_CONTAINER_H
