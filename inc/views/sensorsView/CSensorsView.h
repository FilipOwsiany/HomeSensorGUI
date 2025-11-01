#ifndef C_SENSORS_VIEW_H
#define C_SENSORS_VIEW_H

#include "lvgl/lvgl.h"

#include "CObject.h"
#include "CLVGLCallbackInterface.h"
#include "CCustomCallbackInterface.h"
#include "SObjectContairner.h"

#include "CSensor.h"

class CSensorsView : public CObject, public CLVGLCallbackInterface, public CCustomCallbackInterface
{
private:
    CSensor* mSensors[4] = {nullptr};
public:
    //TODO need to be private
    void callback(CEventBase& aEvent) override;
    CSensorsView(lv_obj_t *aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight);
    ~CSensorsView();
};

#endif // C_SENSORS_VIEW_H