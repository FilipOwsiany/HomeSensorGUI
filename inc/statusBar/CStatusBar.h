#ifndef C_STATUS_BAR_H
#define C_STATUS_BAR_H

#include <stdio.h>
#include <vector>
#include <algorithm>

#include "lvgl/lvgl.h"

#include "CObject.h"
#include "CCallbackInterface.h"
#include "SObjectContairner.h"

LV_IMAGE_DECLARE(cloud);
LV_IMAGE_DECLARE(wifi);
LV_IMAGE_DECLARE(bluetooth);
LV_IMAGE_DECLARE(bell);

class CStatusBar : public CObject, public CCallbackInterface
{
private:
    const std::string mStatusbarIconNames[4] = {
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
public:
    CStatusBar(lv_obj_t * aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight);
    ~CStatusBar();
};

#endif // C_STATUS_BAR_H