#ifndef C_INFO_BOX_H
#define C_INFO_BOX_H

#include <string>

#include "lvgl/lvgl.h"

#include "CCell.h"
#include "CObject.h"

class CInfoBox : public CObject
{
private:
    lv_obj_t *      mImage      = nullptr;
    lv_obj_t *      mValue      = nullptr;
    uint32_t        mValueData  = 0;
    std::string     mUnit;
public:
    void addImage(const lv_img_dsc_t* aImageDsc);
    void setValue(uint32_t aValue);
    CInfoBox(    
    lv_obj_t * aParent, 
    int32_t aX, 
    int32_t aY, 
    int32_t aWidth, 
    int32_t aHeight,
    std::string aUnit = "",
    const lv_img_dsc_t* aImageDsc = nullptr,
    uint32_t aValue = 0);
    ~CInfoBox();
};

#endif // C_INFO_BOX_H