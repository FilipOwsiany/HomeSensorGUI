#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "CSegfaultHandler.h"
#include "CFrontend.h"
#include "CReceiver.h"
#include "CPirBacklightController.h"
#include "CLogger.h"

CFrontend* app = nullptr;
CReceiver* receiver = nullptr;
CPirBacklightController* pirController = nullptr;

// ============================================================
// APP THREAD
// ============================================================
void* appThreadTask(void* arg)
{
    LOG_DEBUG("Starting appThreadTask");
    app = new CFrontend();
    while (1) 
    {
        app->run();
    }
    LOG_DEBUG("Exiting appThreadTask");
    return NULL;
}

// ============================================================
// RECEIVER THREAD
// ============================================================
void* receiverThreadTask(void* arg)
{
    LOG_DEBUG("Starting receiverThreadTask");
    receiver = new CReceiver();
    LOG_DEBUG("Exiting receiverThreadTask");
    return NULL;
}

// ============================================================
// PIR/BACKLIGHT THREAD
// ============================================================
void* pirThreadTask(void* arg)
{
    LOG_DEBUG("Starting pirThreadTask");
    pirController = new CPirBacklightController(
        "/dev/hcsr501",
        "/sys/class/backlight/11-0045/brightness",
        5 * 60
    );

    if (!pirController->start()) 
    {
        LOG_ERROR("pirThreadTask failed");
        return NULL;
    }

    while (1)
    {
        sleep(1);
    }
    LOG_DEBUG("Exiting pirThreadTask");
    return NULL;
}

// ============================================================
// MAIN
// ============================================================
int main()
{
    pthread_t appPhread;
    pthread_t receiverPhread;
    pthread_t pirThread;

    CLogger::instance().setLevel(CLogger::Level::DEBUG);
    CLogger::instance().setMaxQueueSize(5000);
    CLogger::instance().setMaxLogSize(256);
    CLogger::instance().enableFile("/tmp/frontend.log");

    LOG_INFO("\n\n\nApplication started\n\n\n");

    // ---------------------------------------
    // APP
    // ---------------------------------------
    LOG_DEBUG("Creating appPhread");
    if (pthread_create(&appPhread, NULL, appThreadTask, NULL) != 0)
    {
        LOG_ERROR("Failed to create appPhread");
        return 1;
    }

    // ---------------------------------------
    // RECEIVER
    // ---------------------------------------
    LOG_DEBUG("Creating receiverPhread");
    if (pthread_create(&receiverPhread, NULL, receiverThreadTask, NULL) != 0)
    {
        LOG_ERROR("Failed to create receiverPhread");
        return 1;
    }

    // ---------------------------------------
    // PIR CONTROLLER
    // ---------------------------------------
    LOG_DEBUG("Creating pirThread");
    if (pthread_create(&pirThread, NULL, pirThreadTask, NULL) != 0)
    {
        LOG_ERROR("Failed to create pirThread");
        return 1;
    }

    // ---------------------------------------
    // Join threads
    // ---------------------------------------
    pthread_join(appPhread, NULL);
    pthread_join(receiverPhread, NULL);
    pthread_join(pirThread, NULL);

    return 0;
}
