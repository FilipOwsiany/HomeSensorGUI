#include "CGuiApp.h"

#include <unistd.h>
#include <stdio.h>

#include "CDisplayInitializer.h"

#include "CMainViewManager.h"
#include "CSidePanel.h"
#include "CStatusBar.h"
#include "CSensorsView.h"
#include "CLayoutView.h"
#include "CSettingsView.h"
#include "CInfoView.h"
#include "CEvents.h"
#include "IEventCallback.h"
#include "CEventDispatcher.h"
#include "CCustomEvent.h"

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

CGuiApp::CGuiApp()
{
    initLvgl();

    mDisplayInitializer = new CDisplayInitializer();

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x252B3B), 0);

    createUi();
    sendInitialStatusEvents();
    sendInitialSensorEvents();
    registerGlobalCallbacks();
}

CGuiApp::~CGuiApp()
{
    delete mInfoView;
    delete mSettingsView;
    delete mLayoutView;
    delete mSensorsView;
    delete mStatusBar;
    delete mSidePanel;
    delete mMainViewManager;
    delete mDisplayInitializer;
}

void CGuiApp::initLvgl()
{
    lv_init();
}

void CGuiApp::createUi()
{
    mMainViewManager = new CMainViewManager();

    mSidePanel = new CSidePanel(*mMainViewManager,
                                lv_screen_active(),
                                20, 20, 180, 680);

    mStatusBar = new CStatusBar(lv_screen_active(),
                                230, 20, 1030, 80);

    mSensorsView  = new CSensorsView (lv_screen_active(), 230, 130, 1030, 570);
    mLayoutView   = new CLayoutView  (lv_screen_active(), 230, 130, 1030, 570);
    mSettingsView = new CSettingsView(lv_screen_active(), 230, 130, 1030, 570);
    mInfoView     = new CInfoView    (lv_screen_active(), 230, 130, 1030, 570);

    mMainViewManager->registerView(mSensorsView,  0, "Sensors View");
    mMainViewManager->registerView(mLayoutView,   1, "Layout View");
    mMainViewManager->registerView(mSettingsView, 2, "Settings View");
    mMainViewManager->registerView(mInfoView,     3, "Info View");
}

void CGuiApp::sendInitialStatusEvents()
{
    auto* wifiEvent = new CEventWifi();
    wifiEvent->setWifiEventType(CEventWifi::EEventWifiType::CONNECTED);
    mStatusBar->callback(*wifiEvent);

    auto* cloudEvent = new CEventCloud();
    cloudEvent->setCloudEventType(CEventCloud::EEventCloudType::CONNECTED);
    mStatusBar->callback(*cloudEvent);

    auto* bluetoothEvent = new CEventBluetooth();
    bluetoothEvent->setBluetoothEventType(CEventBluetooth::EEventBluetoothType::DISCONNECTED);
    mStatusBar->callback(*bluetoothEvent);
}

void CGuiApp::sendInitialSensorEvents()
{
    {
        auto* ev = new CEventSensorName();
        ev->setSensorId(0);
        ev->setSensorName("Kitchen");
        mSensorsView->callback(*ev);
    }
    {
        auto* ev = new CEventSensorName();
        ev->setSensorId(1);
        ev->setSensorName("Bathroom");
        mSensorsView->callback(*ev);
    }
    {
        auto* ev = new CEventSensorName();
        ev->setSensorId(2);
        ev->setSensorName("Living room");
        mSensorsView->callback(*ev);
    }
    {
        auto* ev = new CEventSensorName();
        ev->setSensorId(3);
        ev->setSensorName("Bedroom");
        mSensorsView->callback(*ev);
    }

    // {
    //     auto* ev = new CEventSensorTemperature();
    //     ev->setSensorId(0);
    //     ev->setSensorTemperature(21);
    //     mSensorsView->callback(*ev);
    // }
    // {
    //     auto* ev = new CEventSensorHumidity();
    //     ev->setSensorId(0);
    //     ev->setSensorHumidity(45);
    //     mSensorsView->callback(*ev);
    // }
    // {
    //     auto* ev = new CEventSensorPressure();
    //     ev->setSensorId(0);
    //     ev->setSensorPressure(101325);
    //     mSensorsView->callback(*ev);
    // }
    // {
    //     auto* ev = new CEventSensorBattery();
    //     ev->setSensorId(0);
    //     ev->setSensorId(0);
    //     ev->setSensorBatteryLevel(3700);
    //     mSensorsView->callback(*ev);
    // }
}


void CGuiApp::registerGlobalCallbacks()
{
    static SampleEventCallback sampleCallback;
    CEventDispatcher::getInstance().registerCallback(&sampleCallback,
                                                     EEventType::Temperature);
}

int CGuiApp::run()
{
    while (true)
    {
        lv_timer_handler();
        usleep(5000);
    }
    return 0;
}
