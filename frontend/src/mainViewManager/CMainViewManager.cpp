#include "CMainViewManager.h"
#include "CLogger.h"

CMainViewManager::CMainViewManager(/* args */)
{
    LOG_DEBUG("CMainViewManager::CMainViewManager() called");
}

CMainViewManager::~CMainViewManager()
{
}

void CMainViewManager::registerView(CObject *aView, uint32_t aId, const std::string &aName)
{
    SObjectContairner<CObject> container;
    container.mObject = aView;
    container.mId = aId;
    container.mName = aName;
    mViewContainers.push_back(container);
}

void CMainViewManager::callback(lv_event_t *aEvent, uint32_t aId)
{
    auto itView = std::find_if(mViewContainers.begin(), mViewContainers.end(),
                               [aId](const SObjectContairner<CObject> &c)
                               {
                                   return c.mId == aId;
                               });

    if (itView != mViewContainers.end())
    {
        for (auto &container : mViewContainers)
        {
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