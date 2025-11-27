#ifndef C_BACKEND_H
#define C_BACKEND_H

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "CHttps.h"
#include "SDataContainer.h"
#include "CEnvReader.h"
#include <string>

class CBackend
{
private:
    CHttps mHttps;
    bool mIsValid;
    SDataContainerAuthorization mAuthorization;
    CEnvReader mEnvReader;
public:
    CBackend(const std::string& aHost);
    ~CBackend();

    bool readEnv();
    bool authorize();
    std::optional<std::vector<SDataContainerSensorMeasurment>> getSensorMeasurments();
    bool isValid() { return mIsValid; };
};

#endif // C_BACKEND_H
