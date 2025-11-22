#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "CSegfaultHandler.h"
#include "CFrontend.h"
#include "CReceiver.h"
#include "CPirBacklightController.h"

CFrontend* app = nullptr;
CReceiver* receiver = nullptr;
CPirBacklightController* pirController = nullptr;

// ============================================================
// APP THREAD
// ============================================================
void* appThreadTask(void* arg)
{
    app = new CFrontend();
    while (1) 
    {
        app->run();
    }
    return NULL;
}

// ============================================================
// RECEIVER THREAD
// ============================================================
void* receiverThreadTask(void* arg)
{
    receiver = new CReceiver();
    return NULL;
}

// ============================================================
// PIR/BACKLIGHT THREAD
// ============================================================
void* pirThreadTask(void* arg)
{
    pirController = new CPirBacklightController(
        "/dev/hcsr501",
        "/sys/class/backlight/11-0045/brightness",
        5 * 60
    );

    if (!pirController->start()) 
    {
        printf("PIR controller failed to start!\n");
        return NULL;
    }

    while (1)
    {
        sleep(1);
    }

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

    // ---------------------------------------
    // APP
    // ---------------------------------------
    if (pthread_create(&appPhread, NULL, appThreadTask, NULL) != 0)
    {
        perror("pthread_create(app)");
        return 1;
    }

    // ---------------------------------------
    // RECEIVER
    // ---------------------------------------
    if (pthread_create(&receiverPhread, NULL, receiverThreadTask, NULL) != 0)
    {
        perror("pthread_create(receiver)");
        return 1;
    }

    // ---------------------------------------
    // PIR CONTROLLER
    // ---------------------------------------
    if (pthread_create(&pirThread, NULL, pirThreadTask, NULL) != 0)
    {
        perror("pthread_create(pirThread)");
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
