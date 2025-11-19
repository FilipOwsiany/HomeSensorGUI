#include "CDisplayInitializer.h"

#include <cstdlib>
#include <stdio.h>

CDisplayInitializer::CDisplayInitializer()
{
    init();
}

void CDisplayInitializer::init()
{
#if BUILDING_FOR_RPI5 == 1
    printf("\n\nBuilding for RPI5\n\n");
    mDisplay = lv_linux_fbdev_create();
    lv_linux_fbdev_set_file(mDisplay,
        getenv("LV_LINUX_FBDEV_DEVICE") ?: "/dev/fb0");
    lv_display_set_rotation(mDisplay, LV_DISPLAY_ROTATION_90);

    mInput = lv_evdev_create(LV_INDEV_TYPE_POINTER,
        getenv("EVDEV_NAME") ?: "/dev/input/event5");
    lv_indev_set_display(mInput, mDisplay);
#elif BUILDING_FOR_PC == 1
    printf("\n\nBuilding for Host\n\n");
    const char *w_env = getenv("LV_SDL_WIDTH");
    const char *h_env = getenv("LV_SDL_HEIGHT");
    int32_t w = w_env ? atoi(w_env) : 1280;
    int32_t h = h_env ? atoi(h_env) : 720;

    extern lv_display_t * lv_sdl_window_create(int32_t width, int32_t height);
    mDisplay = lv_sdl_window_create(w, h);

    extern lv_indev_t * lv_sdl_mouse_create(void);
    mInput = lv_sdl_mouse_create();
    lv_indev_set_display(mInput, mDisplay);
#else
    #error "Define BUILDING_FOR_RPI5 or BUILDING_FOR_PC"
#endif
}
