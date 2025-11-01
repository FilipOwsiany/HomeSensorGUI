#ifndef C_SENSOR_H
#define C_SENSOR_H

#include "lvgl/lvgl.h"

#include "CObject.h"
#include "CLVGLCallbackInterface.h"
#include "CCustomCallbackInterface.h"
#include "SObjectContairner.h"
#include "CInfoBox.h"

LV_IMAGE_DECLARE(thermometer);
LV_IMAGE_DECLARE(humidity);
LV_IMAGE_DECLARE(pressure);
LV_IMAGE_DECLARE(lighting);

class CSensor : public CObject, public CLVGLCallbackInterface, public CCustomCallbackInterface
{
private:
    lv_obj_t *mSensorName = nullptr;
    CInfoBox *mInfoBoxes[4] = { nullptr };

public:
    enum class ESensorInfoBoxIds : uint32_t
    {
        eTemperature = 0,
        eHumidity,
        ePressure,
        eBattery,

        eInfoTileCount
    };

    const std::string mSensorInfoBoxNames[static_cast<uint32_t>(ESensorInfoBoxIds::eInfoTileCount)] = {
        "C",
        "%",
        "Pa",
        "mV"
    };

    const std::string mSensorInfoBoxUnit[static_cast<uint32_t>(ESensorInfoBoxIds::eInfoTileCount)] = {
        "Temperature",
        "Humidity",
        "Pressure",
        "Battery"
    };

    const lv_image_dsc_t* mSensorInfoBoxImages[static_cast<uint32_t>(ESensorInfoBoxIds::eInfoTileCount)] = {
        &thermometer,
        &humidity,
        &pressure,
        &lighting
    };

    //TODO need to be private
    void callback(CEventBase& aEvent) override;
    CSensor(lv_obj_t *aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight);
    ~CSensor();
};

#endif // C_SENSORS_VIEW_H