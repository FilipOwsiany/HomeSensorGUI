#include "CPirBacklightController.h"
#include "CLogger.h"

#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <cstdlib>
#include <iostream>

CPirBacklightController::CPirBacklightController(
    const std::string& pirDevicePath,
    const std::string& backlightBrightness,
    long timeoutSeconds)
    : mPirDevicePath(pirDevicePath)
    , mBacklightBrightnessPath(backlightBrightness)
    , mTimeoutSeconds(timeoutSeconds)
{
}

CPirBacklightController::~CPirBacklightController()
{
    stop();
}

bool CPirBacklightController::start()
{
    if (mRunning.load())
        return true;

    mRunning = true;

    int ret1 = pthread_create(&mThreadPoll, nullptr, &CPirBacklightController::pollThreadEntry, this);
    if (ret1 != 0)
    {
        LOG_ERROR("Failed to create mThreadPoll");
        mRunning = false;
        return false;
    }

    int ret2 = pthread_create(&mThreadControl, nullptr, &CPirBacklightController::controlThreadEntry, this);
    if (ret2 != 0)
    {
        LOG_ERROR("Failed to create mThreadControl");
        mRunning = false;
        pthread_cancel(mThreadPoll);
        pthread_join(mThreadPoll, nullptr);
        return false;
    }

    return true;
}

void CPirBacklightController::stop()
{
    if (!mRunning.load())
        return;

    mRunning = false;

    if (mThreadPoll)
    {
        pthread_join(mThreadPoll, nullptr);
        mThreadPoll = 0;
    }

    if (mThreadControl)
    {
        pthread_join(mThreadControl, nullptr);
        mThreadControl = 0;
    }
}

void* CPirBacklightController::pollThreadEntry(void* arg)
{
    CPirBacklightController* self = static_cast<CPirBacklightController*>(arg);
    self->pollLoop();
    return nullptr;
}

void* CPirBacklightController::controlThreadEntry(void* arg)
{
    CPirBacklightController* self = static_cast<CPirBacklightController*>(arg);
    self->controlLoop();
    return nullptr;
}

void CPirBacklightController::pollLoop()
{
    LOG_INFO("Starting pollLoop");

    int fd = open(mPirDevicePath.c_str(), O_RDONLY);
    if (fd < 0)
    {
        LOGF_ERROR("Failed to open PIR device: %s", mPirDevicePath.c_str());
        mRunning = false;
        return;
    }

    struct pollfd pfd{};
    pfd.fd     = fd;
    pfd.events = POLLIN;

    unsigned char val = 0;

    while (mRunning.load())
    {
        int ret = poll(&pfd, 1, 1000);
        if (ret < 0)
        {
            LOG_ERROR("Poll error on PIR device");
            break;
        }

        if (ret == 0)
        {
            continue;
        }

        if (pfd.revents & POLLIN)
        {
            if (read(fd, &val, 1) == 1)
            {
                mDetectedTimeNs = nowNs();
                mDetected       = true;
                LOGF_DEBUG("PIR detected motion, value=%d", val);
            }
        }
    }

    close(fd);
    LOG_DEBUG("Exiting pollLoop");
}

void CPirBacklightController::controlLoop()
{
    LOGF_DEBUG("Control thread started, timeout set to %ld seconds", mTimeoutSeconds);

    long timeoutCounter = 0;
    long long lastHeartbeatLogNs = 0;
    long long lastCounterLogNs   = 0;
    bool timeoutLogged           = false;

    setBacklight(31);

    while (mRunning.load())
    {
        usleep(100000); // 100 ms

        long long now = nowNs();

        if (now - lastHeartbeatLogNs >= 5LL * 1000 * 1000 * 1000)
        {
            LOGF_DEBUG("Control heartbeat at %lld ns", now);
            lastHeartbeatLogNs = now;
        }

        if (mDetected.load())
        {
            LOG_DEBUG("PIR detected, resetting timeout counter");
            setBacklight(31);
            timeoutCounter = 0;
            mDetected = false;
            timeoutLogged = false;
        }
        else
        {
            if (timeoutCounter < mTimeoutSeconds * 10) // *10 because 10 * 100 ms = 1 s
            {
                timeoutCounter++;
            }
        }

        if (now - lastCounterLogNs >= 1LL * 1000 * 1000 * 1000)
        {
            LOGF_DEBUG("Timeout counter: %ld / %ld seconds",
                       timeoutCounter / 10, mTimeoutSeconds);
            lastCounterLogNs = now;
        }

        if (timeoutCounter >= mTimeoutSeconds * 10)
        {
            if (!timeoutLogged)
            {
                LOGF_INFO("Timeout reached (%ld seconds), dimming backlight", mTimeoutSeconds);
                timeoutLogged = true;
            }
            setBacklight(1);
        }
    }

    LOG_DEBUG("Exiting controlLoop");
}


long long CPirBacklightController::nowNs()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

void CPirBacklightController::setBacklight(int value)
{
    int prev = mCurrentBacklight.load();
    if (prev == value)
        return;

    mCurrentBacklight = value;

    char cmd[256];
    snprintf(cmd, sizeof(cmd),
             "echo %d > %s", value, mBacklightBrightnessPath.c_str());
    int ret = system(cmd);
    if (ret != 0)
    {
        LOGF_ERROR("Failed to set backlight to %d (system returned %d)", value, ret);
    }
    else
    {
        LOGF_INFO("Backlight set to %d", value);
    }
}
