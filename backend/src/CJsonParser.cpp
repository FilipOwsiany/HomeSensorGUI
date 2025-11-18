#include "CJsonParser.h"
#include "json.hpp"

using json = nlohmann::json;

std::optional<SDataContainerAuthorization>
CJsonParser::parseAuthResponse(const std::string& body)
{
    try
    {
        json j = json::parse(body);

        if (!j.contains("token"))
            return std::nullopt;

        SDataContainerAuthorization auth;
        auth.token = j["token"].get<std::string>();

        if (j.contains("refreshToken"))
            auth.refreshToken = j["refreshToken"].get<std::string>();

        if (j.contains("expiresAt"))
            auth.expiresAt = j["expiresAt"].get<long long>();

        return auth;
    }
    catch (...)
    {
        return std::nullopt;
    }
}

std::optional<std::vector<SDataContainerSensorMeasurment>>
CJsonParser::parseSensorMeasurments(const std::string& body)
{
    try
    {
        json j = json::parse(body);

        if (!j.is_array())
            return std::nullopt;

        std::vector<SDataContainerSensorMeasurment> out;
        out.reserve(j.size());

        for (const auto& item : j)
        {
            SDataContainerSensorMeasurment s;
            s.timestamp         = item.at("timestamp").get<long long>();
            s.mTemperature      = item.at("value").get<double>();
            s.mPressure         = item.at("value2").get<double>();
            s.mHumidity         = item.at("value3").get<double>();
            s.mVoltage          = item.at("value4").get<int>();

            out.push_back(s);
        }

        return out;
    }
    catch (...)
    {
        return std::nullopt;
    }
}
