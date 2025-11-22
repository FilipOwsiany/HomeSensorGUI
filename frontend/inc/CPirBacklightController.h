#ifndef C_PIR_BACKLIGHT_CONTROLLER_H
#define C_PIR_BACKLIGHT_CONTROLLER_H

#include <atomic>
#include <string>
#include <thread>

#include <pthread.h>
#include <atomic>
#include <string>

class CPirBacklightController
{
public:
    CPirBacklightController(
        const std::string& pirDevicePath       = "/dev/hcsr501",
        const std::string& backlightBrightness = "/sys/class/backlight/11-0045/brightness",
        long timeoutSeconds                    = 5 * 60   // 5 min
    );

    ~CPirBacklightController();

    bool start();
    void stop();

private:
    static void* pollThreadEntry(void* arg);
    static void* controlThreadEntry(void* arg);

    void pollLoop();
    void controlLoop();

    static long long nowNs();
    void setBacklight(int value);

private:
    std::string mPirDevicePath;
    std::string mBacklightBrightnessPath;
    long        mTimeoutSeconds;

    pthread_t   mThreadPoll     = 0;
    pthread_t   mThreadControl  = 0;
    std::atomic<bool>      mRunning{false};
    std::atomic<bool>      mDetected{false};
    std::atomic<long long> mDetectedTimeNs{0};
    std::atomic<int>       mCurrentBacklight{-1};
};

#endif