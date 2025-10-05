#ifndef C_MAIN_VIEW_MANAGER_H
#define C_MAIN_VIEW_MANAGER_H

#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

#include "lvgl/lvgl.h"

#include "CCallbackInterface.h"
#include "SObjectContairner.h"

class CMainViewManager : public CCallbackInterface
{
private:
    std::vector<SObjectContairner<lv_obj_t>> mViewContainers;
    void callback(lv_event_t * aEvent, uint32_t aId) override;
public:
    CMainViewManager(/* args */);
    ~CMainViewManager();

    void registerView(lv_obj_t * aView, uint32_t aId, const std::string& aName);
};

#endif // C_MAIN_VIEW_MANAGER_H