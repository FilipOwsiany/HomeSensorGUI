#include "CFrontend.h"

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

CFrontend::CFrontend()
{
    initLvgl();

    mDisplayInitializer = new CDisplayInitializer();

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x252B3B), 0);

    createUi();

    auto* wifiEvent = new CEventWifi();
    wifiEvent->setWifiEventType(CEventWifi::EEventWifiType::DISCONNECTED);
    mStatusBar->callback(*wifiEvent);

    auto* cloudEvent = new CEventCloud();
    cloudEvent->setCloudEventType(CEventCloud::EEventCloudType::DISCONNECTED);
    mStatusBar->callback(*cloudEvent);

    auto* bluetoothEvent = new CEventBluetooth();
    bluetoothEvent->setBluetoothEventType(CEventBluetooth::EEventBluetoothType::DISCONNECTED);
    mStatusBar->callback(*bluetoothEvent);

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


    CEventDispatcher::getInstance().registerCallback(this, EEventType::Temperature);
}

CFrontend::~CFrontend()
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

void CFrontend::initLvgl()
{
    lv_init();
}

void CFrontend::createUi()
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

void CFrontend::onEvent(CEventBase* aEvent)
{
    if (!aEvent)
        return;

    auto* wifiEvent = new CEventWifi();
    wifiEvent->setWifiEventType(CEventWifi::EEventWifiType::CONNECTED);
    mStatusBar->callback(*wifiEvent);

    auto* cloudEvent = new CEventCloud();
    cloudEvent->setCloudEventType(CEventCloud::EEventCloudType::CONNECTED);
    mStatusBar->callback(*cloudEvent);

    auto* bluetoothEvent = new CEventBluetooth();
    bluetoothEvent->setBluetoothEventType(CEventBluetooth::EEventBluetoothType::DISCONNECTED);
    mStatusBar->callback(*bluetoothEvent);

    switch (aEvent->getEventType())
    {
        // =====================================================================
        // TEMPERATURE
        // =====================================================================
        case EEventType::Temperature:
        {
            auto* e = static_cast<CEventTemperature*>(aEvent);

            auto* internal = new CEventSensorTemperature();
            internal->setSensorId(0);
            internal->setSensorTemperature(e->getTemperature());

            mSensorsView->callback(*internal);

            std::cout << "[EVENT] Temperature: "
                      << std::dec << e->getTemperature()
                      << " (scaled)\n";

            break;
        }

        // =====================================================================
        // PRESSURE
        // =====================================================================
        case EEventType::Pressure:
        {
            auto* e = static_cast<CEventPressure*>(aEvent);

            auto* internal = new CEventSensorPressure();
            internal->setSensorId(0);
            internal->setSensorPressure(e->getPressure());

            mSensorsView->callback(*internal);

            std::cout << "[EVENT] Pressure: "
                      << std::dec << e->getPressure()
                      << " (scaled)\n";

            break;
        }

        // =====================================================================
        // HUMIDITY
        // =====================================================================
        case EEventType::Humidity:
        {
            auto* e = static_cast<CEventHumidity*>(aEvent);

            auto* internal = new CEventSensorHumidity();
            internal->setSensorId(0);
            internal->setSensorHumidity(e->getHumidity());

            mSensorsView->callback(*internal);

            std::cout << "[EVENT] Humidity: "
                      << std::dec << e->getHumidity()
                      << " (scaled)\n";

            break;
        }

        // =====================================================================
        // VOLTAGE
        // =====================================================================
        case EEventType::Voltage:
        {
            auto* e = static_cast<CEventVoltage*>(aEvent);

            auto* internal = new CEventSensorBattery();
            internal->setSensorId(0);
            internal->setSensorBatteryLevel(e->getVoltage());

            mSensorsView->callback(*internal);

            std::cout << "[EVENT] Voltage: "
                      << std::dec << e->getVoltage()
                      << " (scaled)\n";

            break;
        }

        // =====================================================================
        // UNKNOWN TYPE
        // =====================================================================
        default:
        {
            std::cout << "[EVENT] Unknown event type: "
                      << static_cast<int>(aEvent->getEventType())
                      << "\n";
            break;
        }
    }
}


int CFrontend::run()
{
    while (true)
    {
        lv_timer_handler();
        usleep(5000);
    }
    return 0;
}
