#include "CObject.h"

#define DEBUG_OBJECT 0

CObject::CObject()
{

}

CObject::CObject(lv_obj_t * aParent, int32_t aWidth, int32_t aHeight, int32_t aCol, int32_t aRow)
{
    mObject = lv_obj_create(aParent);
    lv_obj_set_size(mObject, aWidth, aHeight);
    lv_obj_set_pos(mObject, aCol, aRow);
    lv_obj_clear_flag(mObject, LV_OBJ_FLAG_SCROLLABLE); 
    lv_obj_clear_flag(mObject, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_pad_all(mObject, 0, 0);
    lv_obj_set_style_pad_row(mObject, 0, 0);
    lv_obj_set_style_pad_column(mObject, 0, 0);

    #if DEBUG_OBJECT
        lv_obj_t* debugOverlay = lv_obj_create(mObject);
        lv_obj_remove_style_all(debugOverlay);
        lv_obj_add_flag(debugOverlay, LV_OBJ_FLAG_IGNORE_LAYOUT);
        lv_obj_set_align(debugOverlay, LV_ALIGN_TOP_LEFT);
        lv_obj_set_size(debugOverlay, LV_PCT(100), LV_PCT(100));
        lv_obj_set_style_bg_opa(debugOverlay, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(debugOverlay, 1, 0);
        lv_obj_set_style_border_color(debugOverlay, lv_palette_main(LV_PALETTE_RED), 0);
        lv_obj_clear_flag(debugOverlay, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(debugOverlay, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_move_foreground(debugOverlay);
    #endif
}

CObject::~CObject()
{
    if (mObject != nullptr)
    {
        lv_obj_del(mObject);
        mObject = nullptr;
    }
}

void CObject::setPosition(int32_t aX, int32_t aY)
{
    if (mObject != nullptr)
    {
        lv_obj_set_pos(mObject, aX, aY);
    }
}

void CObject::setSize(int32_t aWidth, int32_t aHeight)
{
    if (mObject != nullptr)
    {
        lv_obj_set_size(mObject, aWidth, aHeight);
    }
}
