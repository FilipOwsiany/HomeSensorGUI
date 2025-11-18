#ifndef C_JSON_PARSER_H
#define C_JSON_PARSER_H

#include <string>
#include <vector>
#include <optional>

#include "SDataContainer.h"

class CJsonParser
{
public:

    static std::optional<SDataContainerAuthorization>
    parseAuthResponse(const std::string& body);

    static std::optional<std::vector<SDataContainerSensorMeasurment>>
    parseSensorMeasurments(const std::string& body);
};

#endif
