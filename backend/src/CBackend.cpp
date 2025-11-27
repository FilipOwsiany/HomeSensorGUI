#include "CBackend.h"
#include "CJsonParser.h"

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
        std::cerr << "No response from server during login\n";
        return false;
    }

    std::cout << "Login status: " << loginResponse->status << "\n";
    std::cout << "Login body: "   << loginResponse->body   << "\n";

    if (loginResponse->status != 200)
    {
        std::cerr << "Login failed (status != 200)\n";
        return false;
    }

    auto parsedAuthorization = CJsonParser::parseAuthResponse(loginResponse->body);
    if (!parsedAuthorization)
    {
        std::cerr << "Failed to parse login response (JSON)\n";
        return false;
    }

    mAuthorization = *parsedAuthorization;
    std::cout << "Token: " << mAuthorization.token << "\n";
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
        std::cerr << "No response from server while fetching sensor data\n";
        return std::nullopt;
    }

    std::cout << "Data status: " << dataResponse->status << "\n";

    if (dataResponse->status != 200)
    {
        std::cerr << "Fetching sensor data failed (status != 200)\n";
        std::cerr << "Body: " << dataResponse->body << "\n";
        return std::nullopt;
    }

    return CJsonParser::parseSensorMeasurments(dataResponse->body);
}
