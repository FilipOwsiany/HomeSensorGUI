#ifndef C_MAIN_VIEW_MANAGER_H
#define C_MAIN_VIEW_MANAGER_H

#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

#include "CObject.h"

#include "lvgl/lvgl.h"

#include "CLVGLCallbackInterface.h"
#include "SObjectContairner.h"

class CMainViewManager : public CLVGLCallbackInterface
{
private:
    std::vector<SObjectContairner<CObject>> mViewContainers;

protected:
    void callback(lv_event_t *aEvent, uint32_t aId) override;

public:
    CMainViewManager();
    ~CMainViewManager();

    void registerView(CObject *aView, uint32_t aId, const std::string &aName);
};

#endif // C_MAIN_VIEW_MANAGER_H