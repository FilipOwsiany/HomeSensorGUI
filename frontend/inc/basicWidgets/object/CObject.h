#ifndef C_OBJECT_H
#define C_OBJECT_H

#include <stdio.h>
#include "lvgl/lvgl.h"

class CObject
{
protected:
    lv_obj_t * mObject = nullptr;
public:
    CObject();
    CObject(lv_obj_t * aParent, int32_t aWidth, int32_t aHeight, int32_t aCol, int32_t aRow);
    ~CObject();

    void setPosition(int32_t aX, int32_t aY);
    void setSize(int32_t aWidth, int32_t aHeight);
    void center();
    lv_obj_t * getObject() { return mObject; }
};

#endif // C_OBJECT_H