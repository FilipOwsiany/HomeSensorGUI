#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <syscall.h>
#include <memory.h>
#include <dlfcn.h>
struct sigaction oldSegvSig;
struct sigaction newSegvSig;

#include <unistd.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>


#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"

#include "CSidePanel.h"
#include "CStatusBar.h"
#include "CMainViewManager.h"

#include "CSensorsView.h"
#include "CLayoutView.h"
#include "CSettingsView.h"
#include "CInfoView.h"

#include "CSensor.h"

#include "CCustomEvent.h"

#include "CReceiver.h"
#include "IEventCallback.h"
#include "CEventDispatcher.h"
#include "CEvents.h"

lv_obj_t * objects[4] = {nullptr};

#define BACKTRACE_DEPTH (64)

class SampleEventCallback : public IEventCallback
{
public:
    SampleEventCallback()  = default;
    ~SampleEventCallback() override = default;

    void onEvent(CEventBase* aEvent) override
    {
        if (aEvent->getEventType() == EEventType::Temperature)
        {
            auto* tempEvent = static_cast<CEventTemperature*>(aEvent);
            std::cout << "Temperature: "
                    << std::dec
                    << static_cast<uint32_t>(tempEvent->getTemperature())
                    << " °C\n";


        }
    }
};

void SegvSignalHandler(int signal, siginfo_t *sigInfo, void *context)
{
    // Unblock signal after handler entry (next signals can be handled)
    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s, signal);
    sigprocmask(SIG_UNBLOCK, &s, NULL);

    // get pid of thread(process) which received SIGSEGV signal from OS
    pid_t pid = syscall(SYS_gettid);

    // TODO: [DK] ### TEST CODE - executable must be linked with -rdynamic option (to get symbols
    // name instead of addresses)
    void *backTrace[BACKTRACE_DEPTH] = {0};
    char **backTraceSymbols = 0;
    int backtraceSize = backtrace(backTrace, BACKTRACE_DEPTH);
    backTraceSymbols = backtrace_symbols(backTrace, backtraceSize);

    printf("### stack trace:\n");
    for (int i = 0; i < backtraceSize; i++) {
        Dl_info info;
        if (dladdr(backTrace[i], &info) && info.dli_sname) {
            printf("(%d.) %s : %s\n", i, info.dli_fname, info.dli_sname);
        } else {
            printf("(%d.) %p\n", i, backTrace[i]);
        }
    }
    free(backTraceSymbols);
    printf("### end of stack trace ###\n");

    sigaction(SIGSEGV, &oldSegvSig, NULL);
    exit(0);
}

int main(void)
{
    memset(&oldSegvSig, 0, sizeof(oldSegvSig));
    memset(&newSegvSig, 0, sizeof(newSegvSig));

    newSegvSig.sa_sigaction = SegvSignalHandler;
    newSegvSig.sa_flags = SA_SIGINFO;
    if (sigaction(SIGSEGV, &newSegvSig, &oldSegvSig) == -1)
        printf("::sigaction SIGSEGV set new handler failed!");


    lv_init();

    printf("Starting\n");

    #if BUILDING_FOR_RPI5 == 1
        printf("\n\nBuilding for RPI5\n\n");
        lv_display_t *disp = lv_linux_fbdev_create();
        lv_linux_fbdev_set_file(disp, getenv("LV_LINUX_FBDEV_DEVICE") ?: "/dev/fb0");
        lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);

        lv_indev_t *indev = lv_evdev_create(LV_INDEV_TYPE_POINTER,
                            getenv("EVDEV_NAME") ?: "/dev/input/event5");
        lv_indev_set_display(indev, disp);
    #endif

    #if BUILDING_FOR_PC == 1
        printf("\n\nBuilding for Host\n\n");
        const char *w_env = getenv("LV_SDL_WIDTH");
        const char *h_env = getenv("LV_SDL_HEIGHT");
        int32_t w = w_env ? atoi(w_env) : 1280;
        int32_t h = h_env ? atoi(h_env) : 720;

        extern lv_display_t * lv_sdl_window_create(int32_t width, int32_t height);
        lv_display_t *disp = lv_sdl_window_create(w, h);

        extern lv_indev_t * lv_sdl_mouse_create(void);
        lv_indev_t *indev = lv_sdl_mouse_create();
        lv_indev_set_display(indev, disp);
    #endif

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x252B3B), 0);

    CMainViewManager* mainViewManager = new CMainViewManager();
    CSidePanel * panel = new CSidePanel(*mainViewManager, lv_screen_active(), 20, 20, 180, 680);
    CStatusBar * statusBar = new CStatusBar(lv_screen_active(), 230, 20, 1030, 80);

    CSensorsView* sensorsView = new CSensorsView(lv_screen_active(), 230, 130, 1030, 570);
    CLayoutView* layoutView = new CLayoutView(lv_screen_active(), 230, 130, 1030, 570);
    CSettingsView* settingsView = new CSettingsView(lv_screen_active(), 230, 130, 1030, 570);
    CInfoView* infoView = new CInfoView(lv_screen_active(), 230, 130, 1030, 570);


    mainViewManager->registerView(sensorsView,  0,     "Sensors View");
    mainViewManager->registerView(layoutView,   1,     "Layout View");
    mainViewManager->registerView(settingsView, 2,     "Settings View");
    mainViewManager->registerView(infoView,     3,     "Info View");

    CEventWifi* wifiEvent = new CEventWifi();
    wifiEvent->setWifiEventType(CEventWifi::EEventWifiType::CONNECTED);
    statusBar->callback(*wifiEvent);

    CEventCloud* cloudEvent = new CEventCloud();
    cloudEvent->setCloudEventType(CEventCloud::EEventCloudType::CONNECTED);
    statusBar->callback(*cloudEvent);

    CEventBluetooth* bluetoothEvent = new CEventBluetooth();
    bluetoothEvent->setBluetoothEventType(CEventBluetooth::EEventBluetoothType::DISCONNECTED);
    statusBar->callback(*bluetoothEvent);


    CEventSensorName* sensorNameKitchen = new CEventSensorName();
    sensorNameKitchen->setSensorId(0);
    sensorNameKitchen->setSensorName("Kitchen");
    sensorsView->callback(*sensorNameKitchen);

    CEventSensorName* sensorNameBathroom = new CEventSensorName();
    sensorNameBathroom->setSensorId(1);
    sensorNameBathroom->setSensorName("Bathroom");
    sensorsView->callback(*sensorNameBathroom);

    CEventSensorName* sensorNameLivingRoom = new CEventSensorName();
    sensorNameLivingRoom->setSensorId(2);
    sensorNameLivingRoom->setSensorName("Living room");
    sensorsView->callback(*sensorNameLivingRoom);

    CEventSensorName* sensorNameBedroom = new CEventSensorName();
    sensorNameBedroom->setSensorId(3);
    sensorNameBedroom->setSensorName("Bedroom");
    sensorsView->callback(*sensorNameBedroom);


    CEventSensorTemperature* sensorTemperature = new CEventSensorTemperature();
    sensorTemperature->setSensorId(0);
    sensorTemperature->setSensorTemperature(21);
    sensorsView->callback(*sensorTemperature);

    CEventSensorHumidity* sensorHumidity = new CEventSensorHumidity();
    sensorHumidity->setSensorId(0);
    sensorHumidity->setSensorHumidity(45);
    sensorsView->callback(*sensorHumidity); 

    CEventSensorPressure* sensorPressure = new CEventSensorPressure();
    sensorPressure->setSensorId(0);
    sensorPressure->setSensorPressure(101325);
    sensorsView->callback(*sensorPressure);

    CEventSensorBattery* sensorBattery = new CEventSensorBattery();
    sensorBattery->setSensorId(0);
    sensorBattery->setSensorBatteryLevel(3700);
    sensorsView->callback(*sensorBattery);

    // CReceiver receiver;

    SampleEventCallback sampleCallback;
    CEventDispatcher::getInstance().registerCallback(&sampleCallback,
                                                     EEventType::Temperature);

    while (1) 
    {
        lv_timer_handler();
        usleep(5000);
    }
    return 0;
}
