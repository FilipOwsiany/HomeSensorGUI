// test_hcsr.c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

pthread_t threadPoll     = 0;
pthread_t threadControl  = 0;

long long detectedTimeNs = 0;
bool      detected       = false;

long timeoutInSecs = 5 * 1 * 10; // 5 minutes
long timeoutCounter = 0;

static inline long long now_ns(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

void* pollTask(void* arg) 
{
    printf("Poll thread started\n");
    int fd = open("/dev/hcsr501", O_RDONLY);
    if (fd < 0) 
    {
        perror("open");
       return 1;
    }

    struct pollfd pfd = 
    {
        .fd = fd,
        .events = POLLIN,
    };
    unsigned char val;

    while (1) 
    {
        int ret = poll(&pfd, 1, -1);
        if (ret < 0) 
        {
            perror("poll");
            break;
        }
        if (pfd.revents & POLLIN) 
        {
            if (read(fd, &val, 1) == 1) 
            {
                detectedTimeNs = now_ns();
                detected       = true;
                printf("PIR event -> %u\n", val);
                fflush(stdout);
            }
        }
    }

    close(fd);
    return 0;
}

void* pollControl(void* arg) 
{
    printf("Control thread started, timeout set to %ld seconds\n", timeoutInSecs);
    while (1) 
    {
        usleep(100000); // 100 ms
        printf("Control heartbeat at %lld ns\n", now_ns());

        if (detected)
        {
            printf("PIR detected, resetting timeout counter\n");
            system("echo 31 > /sys/class/backlight/11-0045/brightness");
            timeoutCounter = 0;
            detected = false;
        }
        
        if (timeoutCounter < timeoutInSecs)
        {
            timeoutCounter++;
        }
        
        printf("Timeout counter: %ld / %ld seconds\n", timeoutCounter, timeoutInSecs);

        if (timeoutCounter >= timeoutInSecs) 
        {
            printf("Timeout reached (%ld seconds)\n", timeoutInSecs);
            system("echo 1 > /sys/class/backlight/11-0045/brightness");
        }
    }

    return 0;
}

int main(void)
{
    printf("Starting PIR poll test\n");
    pthread_create(&threadPoll, NULL, pollTask, NULL);
    pthread_create(&threadControl, NULL, pollControl, NULL);

    pthread_join(threadPoll, NULL);
    pthread_join(threadControl, NULL);

    return 0;
}
