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

lv_obj_t * objects[4] = {nullptr};

#define BACKTRACE_DEPTH (64)

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

void lv_example_style_5(void)
{
    static lv_style_t style;
    lv_style_init(&style);

    /* Tło obiektu */
    lv_style_set_radius(&style, 5);
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    lv_style_set_bg_color(&style, lv_palette_lighten(LV_PALETTE_GREY, 1));

    /* Cień — kluczowe: krycie i jasne tło za obiektem */
    lv_style_set_shadow_width(&style, 55);
    lv_style_set_shadow_color(&style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_shadow_opa(&style, LV_OPA_100);   // pełne krycie
    // opcjonalnie dodać przesunięcie:
    // lv_style_set_shadow_ofs_x(&style, 8);
    // lv_style_set_shadow_ofs_y(&style, 8);

    /* Upewnij się, że ekran nie jest czarny */
    lv_obj_t * scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    /* Obiekt z powyższym stylem */
    lv_obj_t * obj = lv_obj_create(scr);
    lv_obj_add_style(obj, &style, 0);
    lv_obj_center(obj);
}


static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = static_cast<lv_obj_t *>(lv_event_get_target(e));
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);

        // if (lv_obj_has_flag(obj1, LV_OBJ_FLAG_HIDDEN)) {
        //     lv_obj_clear_flag(obj1, LV_OBJ_FLAG_HIDDEN);   /*Show the object if it was hidden*/
        // } else
        // {
        //     lv_obj_add_flag(obj1, LV_OBJ_FLAG_HIDDEN);   /*Hide the object if it was visible*/
        // }
    }
}

void lv_example_get_started_2(void)
{
    /* --- Button 1 --- */
    lv_obj_t * btn1 = lv_button_create(lv_screen_active());
    lv_obj_set_pos(btn1, 10, 10);
    lv_obj_set_size(btn1, 120, 50);
    lv_obj_add_event_cb(btn1, btn_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "Button 1");
    lv_obj_center(label1);

    /* --- Button 2 --- */
    lv_obj_t * btn2 = lv_button_create(lv_screen_active());
    lv_obj_set_pos(btn2, 140, 10);
    lv_obj_set_size(btn2, 120, 50);
    lv_obj_add_event_cb(btn2, btn_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "Button 2");
    lv_obj_center(label2);

    /* --- Button 3 --- */
    lv_obj_t * btn3 = lv_button_create(lv_screen_active());
    lv_obj_set_pos(btn3, 270, 10);
    lv_obj_set_size(btn3, 120, 50);
    lv_obj_add_event_cb(btn3, btn_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * label3 = lv_label_create(btn3);
    lv_label_set_text(label3, "Button 3");
    lv_obj_center(label3);
}

void lv_example_style_1(void)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, 5);

    /*Make a gradient*/
    lv_style_set_width(&style, 150);
    lv_style_set_height(&style, LV_SIZE_CONTENT);

    lv_style_set_pad_ver(&style, 20);
    lv_style_set_pad_left(&style, 5);

    lv_style_set_x(&style, lv_pct(50));
    lv_style_set_y(&style, 80);

    /*Create an object with the new style*/
    lv_obj_t * obj = lv_obj_create(lv_screen_active());
    lv_obj_add_style(obj, &style, 0);

    lv_obj_t * label = lv_label_create(obj);
    lv_obj_align(label, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_label_set_text(label, "Hello");
}

void lv_example_image_1(void)
{
    LV_IMAGE_DECLARE(garlic);
    lv_obj_t * img1 = lv_image_create(lv_screen_active());
    lv_image_set_src(img1, &garlic);
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
    lv_image_set_scale(img1, 16); // 50%

    lv_obj_t * img2 = lv_image_create(lv_screen_active());
    lv_image_set_src(img2, LV_SYMBOL_OK "Accept");
    lv_obj_align_to(img2, img1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
}

void lv_example_image_2(void)
{
    LV_IMAGE_DECLARE(img_cogwheel_argb);
    lv_obj_t * img1 = lv_image_create(lv_screen_active());
    lv_image_set_src(img1, &img_cogwheel_argb);
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
    lv_image_set_scale(img1, 16); // 50%

    lv_obj_t * img2 = lv_image_create(lv_screen_active());
    lv_image_set_src(img2, LV_SYMBOL_OK "Accept");
    lv_obj_align_to(img2, img1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
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

    while (1) 
    {
        lv_timer_handler();
        usleep(5000);
    }
    return 0;
}
