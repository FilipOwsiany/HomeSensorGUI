#include "CPirBacklightController.h"

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
        perror("pthread_create pollThread");
        mRunning = false;
        return false;
    }

    int ret2 = pthread_create(&mThreadControl, nullptr, &CPirBacklightController::controlThreadEntry, this);
    if (ret2 != 0)
    {
        perror("pthread_create controlThread");
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
    std::cout << "Poll thread started\n";

    int fd = open(mPirDevicePath.c_str(), O_RDONLY);
    if (fd < 0)
    {
        perror("open PIR device");
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
            perror("poll");
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
                printf("PIR event -> %u\n", val);
                fflush(stdout);
            }
        }
    }

    close(fd);
    std::cout << "Poll thread exiting\n";
}

void CPirBacklightController::controlLoop()
{
    std::cout << "Control thread started, timeout set to "
              << mTimeoutSeconds << " seconds\n";

    long timeoutCounter = 0;

    setBacklight(31);

    while (mRunning.load())
    {
        usleep(100000); // 100 ms
        printf("Control heartbeat at %lld ns\n", nowNs());

        if (mDetected.load())
        {
            printf("PIR detected, resetting timeout counter\n");
            setBacklight(31);
            timeoutCounter = 0;
            mDetected = false;
        }
        else
        {
            if (timeoutCounter < mTimeoutSeconds * 10) // *10 because 10 * 100ms = 1s
            {
                timeoutCounter++;
            }
        }

        printf("Timeout counter: %ld / %ld seconds\n",
               timeoutCounter / 10, mTimeoutSeconds);

        if (timeoutCounter >= mTimeoutSeconds * 10)
        {
            printf("Timeout reached (%ld seconds)\n", mTimeoutSeconds);
            setBacklight(1);
        }
    }

    std::cout << "Control thread exiting\n";
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
        perror("system(setBacklight)");
    }
}
