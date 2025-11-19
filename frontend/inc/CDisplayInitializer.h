#ifndef C_DISPLAY_INITIALIZER_H
#define C_DISPLAY_INITIALIZER_H

#include "lvgl/lvgl.h"

class CDisplayInitializer
{
public:
    CDisplayInitializer();
    ~CDisplayInitializer() = default;

    lv_display_t* getDisplay() const { return mDisplay; }
    lv_indev_t*   getInput()   const { return mInput; }

private:
    void init();

    lv_display_t* mDisplay {nullptr};
    lv_indev_t*   mInput   {nullptr};
};

#endif
