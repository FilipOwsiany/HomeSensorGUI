#include "CObject.h"

CObject::CObject()
{

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
