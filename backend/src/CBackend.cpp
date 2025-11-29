#include "CBackend.h"
#include "CJsonParser.h"
#include "CLogger.h"

CBackend::CBackend(const std::string& aHost) : mHttps(aHost), mIsValid(false), mEnvReader("env.txt", "/home/")
{
    if (!mHttps.isValid())
    {
        mIsValid = false;
    }
    else
    {
        mIsValid = true;
    }
}

CBackend::~CBackend()
{
}

bool CBackend::readEnv()
{
    mEnvReader.openFile();
    if (!mEnvReader.isGoodFile())
    {
        return false;
    }
    return mEnvReader.readFile();;
}

bool CBackend::authorize()
{
    const std::string loginUrl   = mEnvReader.getEnvReadedData().mLoginUrl;
    const std::string email      = mEnvReader.getEnvReadedData().mEmail;
    const std::string password   = mEnvReader.getEnvReadedData().mPassword;

    CHttps::Headers loginHeaders = {
        { "Content-Type", "application/json" },
        { "Accept",       "application/json" }
    };

    std::string loginBody =
        std::string("{\"email\":\"") + email +
        "\",\"password\":\"" + password + "\"}";

    CHttps::Result loginResponse = mHttps.Post(loginUrl,
                                               loginBody,
                                               "application/json",
                                               loginHeaders);

    if (!loginResponse)
    {
        LOG_ERROR("No response from server during login");
        return false;
    }

    LOGF_DEBUG("Login status: %d", loginResponse->status);
    LOGF_DEBUG("Login body: %s", loginResponse->body.c_str());

    if (loginResponse->status != 200)
    {
        LOG_ERROR("Login failed (status != 200)");
        return false;
    }

    auto parsedAuthorization = CJsonParser::parseAuthResponse(loginResponse->body);
    if (!parsedAuthorization)
    {
        LOG_ERROR("Failed to parse login response (JSON)");
        return false;
    }

    mAuthorization = *parsedAuthorization;
    LOGF_DEBUG("Token: %s", mAuthorization.token.c_str());
    return true;
}

std::optional<std::vector<SDataContainerSensorMeasurment>> 
CBackend::getSensorMeasurments()
{
    const std::string dataUrl = mEnvReader.getEnvReadedData().mDataUrl;

    CHttps::Headers dataHeaders = {
        { "Content-Type",  "application/json" },
        { "Accept",        "application/json" },
        { "Authorization", "Bearer " + mAuthorization.token }
    };

    CHttps::Result dataResponse = mHttps.Get(dataUrl, dataHeaders);

    if (!dataResponse)
    {
        LOG_ERROR("No response from server while fetching sensor data");
        return std::nullopt;
    }

    LOGF_DEBUG("Data status: %d", dataResponse->status);

    if (dataResponse->status != 200)
    {
        LOG_ERROR("Fetching sensor data failed (status != 200)");
        LOGF_ERROR("Body: %s", dataResponse->body.c_str());
        return std::nullopt;
    }

    return CJsonParser::parseSensorMeasurments(dataResponse->body);
}
