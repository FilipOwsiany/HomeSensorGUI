#ifndef C_GUI_APP_H
#define C_GUI_APP_H

class CMainViewManager;
class CSidePanel;
class CStatusBar;
class CSensorsView;
class CLayoutView;
class CSettingsView;
class CInfoView;
class CDisplayInitializer;

class CGuiApp
{
public:
    CGuiApp();
    ~CGuiApp();

    int run();

private:
    void initLvgl();
    void createUi();
    void sendInitialStatusEvents();
    void sendInitialSensorEvents();
    void registerGlobalCallbacks();

private:
    CDisplayInitializer* mDisplayInitializer {nullptr};

    CMainViewManager* mMainViewManager {nullptr};
    CSidePanel*       mSidePanel       {nullptr};
    CStatusBar*       mStatusBar       {nullptr};

    CSensorsView*  mSensorsView  {nullptr};
    CLayoutView*   mLayoutView   {nullptr};
    CSettingsView* mSettingsView {nullptr};
    CInfoView*     mInfoView     {nullptr};
};

#endif