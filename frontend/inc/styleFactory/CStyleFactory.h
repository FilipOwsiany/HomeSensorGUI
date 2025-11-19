#ifndef C_STYLE_FACTORY_H
#define C_STYLE_FACTORY_H

#include "lvgl/lvgl.h"

class CStyleFactory
{
public:
    // Button
    static void buttonMain      (lv_obj_t* mObject);
    static void buttonLabel     (lv_obj_t* mObject);
    static void buttonImage     (lv_obj_t* mObject);
    // InfoBox
    static void infoBoxMain     (lv_obj_t* mObject);
    static void infoBoxValue    (lv_obj_t* mObject);
    static void infoBoxImage    (lv_obj_t* mObject);
    // Sensor
    static void sensorMain      (lv_obj_t* mObject);
    static void sensorCell      (lv_obj_t* mObject);
    static void sensorName      (lv_obj_t* mObject);
    // Sensors
    static void sensorsMain     (lv_obj_t* mObject);
    // Settings
    static void settingsMain    (lv_obj_t* mObject);
    // Layout
    static void layoutMain      (lv_obj_t* mObject);
    // Info
    static void infoMain        (lv_obj_t* mObject);
};

#endif // C_STYLE_FACTORY_H
