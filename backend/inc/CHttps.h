#ifndef C_HTTPS_H
#define C_HTTPS_H

#include <string>
#include <memory>
#include "httplib.h"

class CHttps
{
public:
    using Headers = httplib::Headers;
    using Result  = httplib::Result;

private:
    std::unique_ptr<httplib::SSLClient> mClient;

public:
    explicit CHttps(const std::string& aHost)
        : mClient(std::make_unique<httplib::SSLClient>(aHost.c_str()))
    {
        // Tak jak w Twoim działającym kodzie:
        mClient->enable_server_certificate_verification(false);
    }

    ~CHttps() = default;

    bool isValid() const
    {
        return static_cast<bool>(mClient);
    }

    Result Post(const std::string& aPath,
                const std::string& aBody,
                const std::string& aContentType,
                const Headers& aHeaders = Headers{})
    {
        if (!mClient) return Result{};
        return mClient->Post(aPath.c_str(),
                             aHeaders,
                             aBody,
                             aContentType.c_str());
    }

    Result Get(const std::string& aPath,
               const Headers& aHeaders = Headers{})
    {
        if (!mClient) return Result{};
        return mClient->Get(aPath.c_str(), aHeaders);
    }
};

#endif // C_HTTPS_H
