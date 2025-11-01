#include "CMainViewManager.h"

CMainViewManager::CMainViewManager(/* args */)
{
}

CMainViewManager::~CMainViewManager()
{
}

void CMainViewManager::registerView(CObject *aView, uint32_t aId, const std::string &aName)
{
    printf("CMainViewManager::registerView id: %d, ptr: %p, name: %s\n", aId, aView, aName.c_str());
    SObjectContairner<CObject> container;
    container.mObject = aView;
    container.mId = aId;
    container.mName = aName;
    mViewContainers.push_back(container);
}

void CMainViewManager::callback(lv_event_t *aEvent, uint32_t aId)
{
    printf("CMainViewManager::callback id: %d, ptr: %p\n", aId, aEvent);
    auto itView = std::find_if(mViewContainers.begin(), mViewContainers.end(),
                               [aId](const SObjectContairner<CObject> &c)
                               {
                                   return c.mId == aId;
                               });

    if (itView != mViewContainers.end())
    {
        for (auto &container : mViewContainers)
        {
            lv_obj_t *btn = static_cast<lv_obj_t *>(lv_event_get_target(aEvent));
            if (container.mId == aId)
            {
                lv_obj_clear_flag(container.mObject->getObject(), LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_add_flag(container.mObject->getObject(), LV_OBJ_FLAG_HIDDEN);
            }
        }
    }
}