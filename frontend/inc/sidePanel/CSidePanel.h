#ifndef C_SIDE_PANEL_H
#define C_SIDE_PANEL_H

#include <stdio.h>
#include <vector>
#include <algorithm>

#include "lvgl/lvgl.h"

#include "CObject.h"
#include "CButton.h"
#include "CLVGLCallbackInterface.h"
#include "SObjectContairner.h"

LV_IMAGE_DECLARE(home);
LV_IMAGE_DECLARE(layers);
LV_IMAGE_DECLARE(settings);
LV_IMAGE_DECLARE(information);
LV_IMAGE_DECLARE(back);

class CSidePanel : public CObject, public CLVGLCallbackInterface
{
private:
    std::vector<SObjectContairner<CButton>> mButtonContainers;
    CLVGLCallbackInterface& itsCallbackInterface;
    void callback(lv_event_t * aEvent, uint32_t aId) override;
public:
    enum class ESidePanelButtonIds : uint32_t
    {
        eHome = 0,
        eLayout,
        eSettings,
        eInfo,

        eBack,

        eButtonCount
    };

    const std::string mSidePanelButtonNames[static_cast<uint32_t>(ESidePanelButtonIds::eButtonCount)] = {
        "Home",
        "Layout",
        "Settings",
        "Info",
        "Back"
    };

    const lv_image_dsc_t* mSidePanelButtonImages[static_cast<uint32_t>(ESidePanelButtonIds::eButtonCount)] = {
        &home,
        &layers,
        &settings,
        &information,
        &back
    };

    CSidePanel(CLVGLCallbackInterface& aCallbackInterface, lv_obj_t * aParent = nullptr, int32_t aX = 0, int32_t aY = 0, int32_t aWidth = 100, int32_t aHeight = 100);
    ~CSidePanel();
};

#endif // C_SIDE_PANEL_H