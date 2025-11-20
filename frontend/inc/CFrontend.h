#ifndef C_FRONTEND_H
#define C_FRONTEND_H

#include "IEventCallback.h"

class CMainViewManager;
class CSidePanel;
class CStatusBar;
class CSensorsView;
class CLayoutView;
class CSettingsView;
class CInfoView;
class CDisplayInitializer;

class CFrontend : public IEventCallback
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
    CDisplayInitializer* mDisplayInitializer    {nullptr};

    CMainViewManager* mMainViewManager          {nullptr};
    CSidePanel*       mSidePanel                {nullptr};
    CStatusBar*       mStatusBar                {nullptr};

    CSensorsView*  mSensorsView                 {nullptr};
    CLayoutView*   mLayoutView                  {nullptr};
    CSettingsView* mSettingsView                {nullptr};
    CInfoView*     mInfoView                    {nullptr};
};

#endif