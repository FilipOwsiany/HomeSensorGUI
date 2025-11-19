#include "CObject.h"

CObject::CObject()
{

}

CObject::CObject(lv_obj_t * aParent, int32_t aWidth, int32_t aHeight, int32_t aCol, int32_t aRow)
{
    mObject = lv_obj_create(aParent);
    lv_obj_set_size(mObject, aWidth, aHeight);
    lv_obj_set_pos(mObject, aCol, aRow);
    lv_obj_update_layout(mObject);
    lv_obj_clear_flag(mObject, LV_OBJ_FLAG_SCROLLABLE); 
    lv_obj_clear_flag(mObject, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_pad_all(mObject, 0, 0);
    lv_obj_set_style_pad_row(mObject, 0, 0);
    lv_obj_set_style_pad_column(mObject, 0, 0);
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
        lv_obj_update_layout(mObject);
    }
}

void CObject::setSize(int32_t aWidth, int32_t aHeight)
{
    if (mObject != nullptr)
    {
        lv_obj_set_size(mObject, aWidth, aHeight);
        lv_obj_update_layout(mObject);
    }
}

void CObject::center()
{
    if (mObject != nullptr)
    {
        lv_obj_center(mObject);
    }
}
