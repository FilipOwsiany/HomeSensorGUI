#ifndef CBUTTON_H
#define CBUTTON_H

#include <string>

#include "lvgl/lvgl.h"
#include "CObject.h"
#include "CLVGLCallbackInterface.h" 

class CButton : public CObject
{
private:

    static constexpr int32_t mImageWidth      = 50; 
    static constexpr int32_t mTextMargin      = 10; 
    static constexpr int32_t mInnerPadding    = 5;

    uint32_t mId = 0;
    lv_obj_t * mImage = nullptr;
    lv_obj_t * mLabel = nullptr;
    CLVGLCallbackInterface& itsCallbackInterface;
    static void eventCallback(lv_event_t * aEvent);
public:
    CButton(uint32_t aId, 
            CLVGLCallbackInterface& aCallbackInterface, 
            lv_obj_t * aParent = nullptr, 
            int32_t aX = 0, 
            int32_t aY = 0, 
            int32_t aWidth = 0, 
            int32_t aHeight = 0, 
            std::string aText = "",
            const lv_img_dsc_t* aImageDsc = nullptr);
    ~CButton();

    void addImage(const lv_img_dsc_t* aImageDsc);
    void addText(const std::string aText);
    lv_obj_t * getImage() { return mImage; }
    lv_obj_t * getLabel() { return mLabel; }
};

#endif // CBUTTON_H