#ifndef C_SETTINGS_VIEW_H
#define C_SETTINGS_VIEW_H

#include "lvgl/lvgl.h"

#include "CObject.h"
#include "CLVGLCallbackInterface.h"
#include "CCustomCallbackInterface.h"
#include "SObjectContairner.h"

class CSettingsView : public CObject, public CLVGLCallbackInterface, public CCustomCallbackInterface
{
private:
    /* data */
public:
    //TODO need to be private
    void callback(CCustomEventBase& aEvent) override;
    CSettingsView(lv_obj_t *aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight);
    ~CSettingsView();
};

#endif // C_SETTINGS_VIEW_H