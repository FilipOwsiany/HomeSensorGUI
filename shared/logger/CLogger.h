#ifndef C_LOGGER_H
#define C_LOGGER_H

#include <cstdarg>
#include <string>
#include <queue>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <sstream>
#include <iostream>
#include <chrono>
#include <ctime>

#define LOG_DEBUG(msg)   CLogger::instance().log(CLogger::Level::DEBUG,   msg)
#define LOG_INFO(msg)    CLogger::instance().log(CLogger::Level::INFO,    msg)
#define LOG_WARN(msg)    CLogger::instance().log(CLogger::Level::WARNING, msg)
#define LOG_ERROR(msg)   CLogger::instance().log(CLogger::Level::ERROR,   msg)

#define LOGF_DEBUG(fmt, ...)  CLogger::instance().logf(CLogger::Level::DEBUG,   fmt, ##__VA_ARGS__)
#define LOGF_INFO(fmt, ...)   CLogger::instance().logf(CLogger::Level::INFO,    fmt, ##__VA_ARGS__)
#define LOGF_WARN(fmt, ...)   CLogger::instance().logf(CLogger::Level::WARNING, fmt, ##__VA_ARGS__)
#define LOGF_ERROR(fmt, ...)  CLogger::instance().logf(CLogger::Level::ERROR,   fmt, ##__VA_ARGS__)

class CLogger
{
public:
    enum class Level { DEBUG, INFO, WARNING, ERROR };

    static CLogger& instance()
    {
        static CLogger inst;
        return inst;
    }

    void setLevel(Level level)
    {
        mMinLevel.store(level);
    }

    void setMaxQueueSize(size_t size)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMaxQueueSize = size;
    }

    void setMaxLogSize(size_t size)
    {
        mMaxLogSize = size;
    }

    void enableFile(const std::string& path)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mFile.open(path, std::ios::out | std::ios::app);
    }

    void log(Level level, const std::string& msg)
    {
        if (level < mMinLevel.load())
            return;

        std::ostringstream oss;
        oss << timestamp() << " [" << levelToString(level) << "] " << msg;
        std::string line = oss.str();

        if (line.size() > mMaxLogSize)
        {
            if (mMaxLogSize > 3)
            {
                line.resize(mMaxLogSize - 3);
                line += "...";
            }
            else
            {
                line.resize(mMaxLogSize);
            }
        }

        {
            std::lock_guard<std::mutex> lock(mMutex);

            if (mQueue.size() >= mMaxQueueSize)
            {
                mQueue.pop();
            }

            mQueue.push(std::move(line));
        }

        mCond.notify_one();
    }

    void logf(Level level, const char* fmt, ...)
    {
        if (level < mMinLevel.load())
            return;

        char buffer[1024];

        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        log(level, std::string(buffer));
    }

    void stop()
    {
        bool expected = true;
        if (!mRunning.compare_exchange_strong(expected, false))
            return;

        mCond.notify_one();
        if (mThread.joinable())
            mThread.join();
    }

private:
    CLogger()
        : mRunning(true)
        , mMinLevel(Level::DEBUG)
        , mMaxQueueSize(10000)
        , mMaxLogSize(512)
        , mThread(&CLogger::writerLoop, this)
    {}

    ~CLogger()
    {
        stop();
    }

    std::string timestamp()
    {
        using namespace std::chrono;
        auto now = system_clock::now();
        std::time_t t = system_clock::to_time_t(now);

        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        return buf;
    }

    const char* levelToString(Level lvl)
    {
        switch (lvl)
        {
            case Level::DEBUG:   return "DBG";
            case Level::INFO:    return "INF";
            case Level::WARNING: return "WRN";
            case Level::ERROR:   return "ERR";
        }
        return "?";
    }

    void writerLoop()
    {
        while (mRunning || !queueEmpty())
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mCond.wait(lock, [&]
            {
                return !mQueue.empty() || !mRunning;
            });

            while (!mQueue.empty())
            {
                std::string line = std::move(mQueue.front());
                mQueue.pop();
                lock.unlock();

                std::cout << line << std::endl;

                if (mFile.is_open())
                {
                    mFile << line << std::endl;
                }

                lock.lock();
            }
        }
    }

    bool queueEmpty()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        return mQueue.empty();
    }

private:
    std::atomic<bool>  mRunning;
    std::atomic<Level> mMinLevel;

    size_t mMaxQueueSize;
    size_t mMaxLogSize;

    std::thread mThread;

    std::mutex mMutex;
    std::condition_variable mCond;

    std::queue<std::string> mQueue;
    std::ofstream mFile;
};

#endif // C_LOGGER_H
