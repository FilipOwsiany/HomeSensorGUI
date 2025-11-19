#include "CButton.h"
#include "CCell.h"
#include "CStyleFactory.h"

CButton::CButton(uint32_t aId, 
    CLVGLCallbackInterface& aCallbackInterface, 
    lv_obj_t * aParent, 
    int32_t aX, 
    int32_t aY, 
    int32_t aWidth, 
    int32_t aHeight, 
    std::string aText,
    const lv_img_dsc_t* aImageDsc) : 
    itsCallbackInterface(aCallbackInterface),
    mId(aId)
{
    if (aParent == nullptr)
    {
        return;
    }

    mObject = lv_button_create(aParent);

    setPosition(aX, aY);
    setSize(aWidth, aHeight);
    center();

    CStyleFactory::buttonMain(mObject);

    lv_obj_add_event_cb(mObject, eventCallback, LV_EVENT_PRESSED, this);

    static int32_t col_dsc[] = {
        CButton::mImageWidth,
        lv_obj_get_width(mObject) - CButton::mImageWidth - CButton::mTextMargin,
        LV_GRID_TEMPLATE_LAST
    };

    static int32_t row_dsc[] = {
        LV_GRID_FR(1),
        LV_GRID_TEMPLATE_LAST
    };

    lv_obj_set_style_grid_column_dsc_array(mObject, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(mObject, row_dsc, 0);

    CCell* labelCell = new CCell(
        mObject,
        lv_obj_get_width(mObject) - CButton::mImageWidth - CButton::mTextMargin,
        lv_obj_get_height(mObject) - CButton::mInnerPadding,
        1,
        0
    );

    mLabel = lv_label_create(labelCell->getObject());
    lv_label_set_text_fmt(mLabel, "%s", aText.c_str());

    CStyleFactory::buttonLabel(mObject);

    lv_obj_align_to(
        mLabel,
        mObject,
        LV_ALIGN_LEFT_MID,
        CButton::mImageWidth + CButton::mInnerPadding,
        0
    );

    CCell* imageCell = new CCell(
        mObject,
        CButton::mImageWidth,
        lv_obj_get_height(mObject) - CButton::mInnerPadding,
        0,
        0
    );

    mImage = lv_image_create(imageCell->getObject());

    if (aImageDsc != nullptr)
    {
        lv_image_set_src(mImage, aImageDsc);
    }

    CStyleFactory::buttonImage(mObject);
    lv_obj_center(mImage);
}

CButton::~CButton()
{
}

void CButton::addImage(const lv_img_dsc_t* aImageDsc)
{
    if (mObject == nullptr)
    {
        return;
    }

    if (aImageDsc != nullptr)
    {
        lv_image_set_src(mImage, aImageDsc);
    }
}

void CButton::addText(const std::string aText)
{
    lv_label_set_text_fmt(mLabel, "%s", aText.c_str());
}

void CButton::eventCallback(lv_event_t * aEvent)
{
    CButton* btn = static_cast<CButton *>(lv_event_get_user_data(aEvent));
    if (btn == nullptr)
    {
        return;
    }

    btn->itsCallbackInterface.callback(aEvent, btn->mId);
}
