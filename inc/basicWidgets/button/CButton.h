#ifndef CBUTTON_H
#define CBUTTON_H

#include <string>

#include "lvgl/lvgl.h"
#include "CObject.h"
#include "CCallbackInterface.h" 

class CButton : public CObject
{
private:
    uint32_t mId = 0;
    lv_obj_t * mImage = nullptr;
    lv_obj_t * mLabel = nullptr;
    CCallbackInterface& itsCallbackInterface;
    static void eventCallback(lv_event_t * aEvent);
public:
    CButton(uint32_t aId, 
            CCallbackInterface& aCallbackInterface, 
            lv_obj_t * aParent = nullptr, 
            int32_t aX = 0, 
            int32_t aY = 0, 
            int32_t aWidth = 0, 
            int32_t aHeight = 0, 
            std::string aText = "");
    ~CButton();

    void addImage(const lv_img_dsc_t* aImageDsc);
    lv_obj_t * getImage() { return mImage; }
    lv_obj_t * getLabel() { return mLabel; }
};

#endif // CBUTTON_H