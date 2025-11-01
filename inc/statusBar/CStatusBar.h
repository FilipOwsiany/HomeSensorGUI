#ifndef C_STATUS_BAR_H
#define C_STATUS_BAR_H

#include <stdio.h>
#include <vector>
#include <algorithm>

#include "lvgl/lvgl.h"

#include "CObject.h"
#include "CLVGLCallbackInterface.h"
#include "CCustomCallbackInterface.h"
#include "SObjectContairner.h"

LV_IMAGE_DECLARE(cloud);
LV_IMAGE_DECLARE(wifi);
LV_IMAGE_DECLARE(bluetooth);
LV_IMAGE_DECLARE(bell);

class CStatusBar : public CObject, public CLVGLCallbackInterface, public CCustomCallbackInterface
{
private:

    enum class EIconImageIds : uint32_t
    {
        eCloud = 0,
        eWifi,
        eBluetooth,
        eBell,

        eIconImageCount
    };

    const std::string mStatusbarIconImageNames[4] = {
        "Cloud",
        "Wifi",
        "Bluetooth",
        "Bell"
    };

    const lv_image_dsc_t* mStatusbarIconImages[4] = {
        &cloud,
        &wifi,
        &bluetooth,
        &bell
    };

    //std::vector<SObjectContairner<lv_obj_t>> mIconImagesContainers;

    lv_obj_t* mIconImagesObjects[static_cast<uint32_t>(EIconImageIds::eIconImageCount)] = {nullptr};

    public:
    //TODO need to be private
    void callback(CEventBase& aEvent) override;
    CStatusBar(lv_obj_t * aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight);
    ~CStatusBar();
};

#endif // C_STATUS_BAR_H