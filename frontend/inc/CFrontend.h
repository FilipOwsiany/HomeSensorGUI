#ifndef C_FRONTEND_H
#define C_FRONTEND_H

#include "IEventCallback.h"
#include "CMainViewManager.h"

class CSidePanel;
class CStatusBar;
class CSensorsView;
class CLayoutView;
class CSettingsView;
class CInfoView;
class CDisplayInitializer;

class CFrontend : public IEventCallback , public CMainViewManager
{
public:
    CFrontend();
    ~CFrontend();

    int run();

private:
    void initLvgl();
    void createUi();

    void onEvent(CEventBase* aEvent) override;

private:

    pthread_mutex_t mutex;
    CDisplayInitializer* mDisplayInitializer    {nullptr};

    // CMainViewManager* mMainViewManager          {nullptr};
    CSidePanel*       mSidePanel                {nullptr};
    CStatusBar*       mStatusBar                {nullptr};

    CSensorsView*  mSensorsView                 {nullptr};
    CLayoutView*   mLayoutView                  {nullptr};
    CSettingsView* mSettingsView                {nullptr};
    CInfoView*     mInfoView                    {nullptr};
};

#endif