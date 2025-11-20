#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "CSegfaultHandler.h"
#include "CFrontend.h"
#include "CReceiver.h"

CFrontend* app = nullptr;
CReceiver* receiver = nullptr;

void* appThreadTask(void* arg)
{
    app = new CFrontend();
    while (1) 
    {
        app->run();
    }
    return NULL;
}

void* receiverThreadTask(void* arg)
{
    receiver = new CReceiver();
    return NULL;
}

int main()
{
    pthread_t appPhread;
    pthread_t receiverPhread;

    int ret = pthread_create(&appPhread, NULL, appThreadTask, NULL);
    if (ret != 0) 
    {
        return 1;
    }

    ret = pthread_create(&receiverPhread, NULL, receiverThreadTask, NULL);
    if (ret != 0) 
    {
        return 1;
    }

    pthread_join(appPhread, NULL);
    pthread_join(receiverPhread, NULL);

    return 0;
}

