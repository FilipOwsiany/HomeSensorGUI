#include "CButton.h"
#include "CCell.h"

#define IMAGE_WIDTH  50


CButton::CButton(uint32_t aId, 
    CCallbackInterface& aCallbackInterface, 
    lv_obj_t * aParent, 
    int32_t aX, 
    int32_t aY, 
    int32_t aWidth, 
    int32_t aHeight, 
    std::string aText) : 
    itsCallbackInterface(aCallbackInterface),
    mId(aId)
{
    if (aParent == nullptr)
    {
        return;
    }

    mObject = lv_button_create(aParent);

    lv_obj_set_pos(mObject, aX, aY);
    lv_obj_set_size(mObject, aWidth, aHeight);
    lv_obj_update_layout(mObject);

    lv_obj_center(mObject);

    lv_obj_add_event_cb(mObject, eventCallback, LV_EVENT_PRESSED, this);

    static int32_t col_dsc[] = {IMAGE_WIDTH, lv_obj_get_width(mObject) - IMAGE_WIDTH - 10, LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_set_style_grid_column_dsc_array(mObject, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(mObject, row_dsc, 0);

    lv_obj_set_style_pad_hor(mObject, 5, 0);
    lv_obj_set_style_pad_ver(mObject, 5, 0);
    lv_obj_set_style_pad_row(mObject, 0, 0);
    lv_obj_set_style_pad_column(mObject, 0, 0);
    lv_obj_set_layout(mObject, LV_LAYOUT_GRID);

    CCell* cell = new CCell(mObject, lv_obj_get_width(mObject) - IMAGE_WIDTH - 10, (lv_obj_get_height(mObject) - 5), 1, 0);

    mLabel = lv_label_create(cell->getObject());
    lv_label_set_text_fmt(mLabel, "%s", aText.c_str());

    LV_FONT_DECLARE(AlanSans_20_Medium);
    lv_obj_set_style_text_font(mLabel, &AlanSans_20_Medium, LV_PART_MAIN);

    //lv_obj_align(mLabel, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_align_to(mLabel, mObject, LV_ALIGN_LEFT_MID, IMAGE_WIDTH + 5, 0);
    // lv_obj_set_style_border_width(mLabel, 1, 0);
    // lv_obj_set_style_border_color(mLabel, lv_palette_main(LV_PALETTE_RED), 0);   
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

    CCell* cell = new CCell(mObject, IMAGE_WIDTH, (lv_obj_get_height(mObject) - 5), 0, 0);

    mImage = lv_image_create(cell->getObject());
    lv_image_set_src(mImage, aImageDsc);
    //lv_image_set_scale(mImage, 16); // 50%
    lv_obj_set_style_img_recolor(mImage, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(mImage, LV_OPA_COVER, LV_PART_MAIN); // 255 = pełny tint
    lv_obj_center(mImage);
}


void CButton::eventCallback(lv_event_t * aEvent)
{
    CButton* btn = static_cast<CButton *>(lv_event_get_user_data(aEvent));

    uint32_t i = 0;
    lv_obj_t * child = lv_obj_get_child(btn->mObject, i);

    if (btn != nullptr)
    {
        btn->itsCallbackInterface.callback(aEvent, btn->mId);
    }
}