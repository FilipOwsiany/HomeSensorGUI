#include "CInfoBox.h"

void CInfoBox::addImage(const lv_img_dsc_t* aImageDsc)
{
    if (mImage != nullptr)
    {
        lv_image_set_src(mImage, aImageDsc);
        lv_obj_set_style_img_recolor(mImage, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_img_recolor_opa(mImage, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_center(mImage);
    }
}

void CInfoBox::setValue(uint32_t aValue)
{
    mValueData = aValue;
    if (mValue != nullptr)
    {
        printf("Setting value: %u\n", aValue);
        if (mUnit.size() > 0)
        {
            lv_label_set_text_fmt(mValue, "%d %s", aValue, mUnit.c_str());
        }
        else
        {
            lv_label_set_text_fmt(mValue, "%d", aValue);
        }
    }
}

CInfoBox::CInfoBox(    
    lv_obj_t * aParent, 
    int32_t aX, 
    int32_t aY, 
    int32_t aWidth, 
    int32_t aHeight,
    std::string aUnit,
    const lv_img_dsc_t* aImageDsc,
    uint32_t aValue) :
    CObject(aParent, aWidth, aHeight, aX, aY), 
    mValueData(aValue), 
    mUnit(aUnit)
{
    lv_obj_set_style_bg_color(mObject, lv_color_hex(0x0C1826), 0);
    lv_obj_set_style_bg_opa(mObject, LV_OPA_COVER, 0);   // 255;
    lv_obj_set_style_border_width(mObject, 0, 0);

    static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    
    lv_obj_set_layout(mObject, LV_LAYOUT_GRID);
    lv_obj_set_style_grid_column_dsc_array(mObject, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(mObject, row_dsc, 0);

    CCell* imageCell = new CCell(mObject, 0, 0, 0, 0);
    CCell* valueCell = new CCell(mObject, 0, 0, 1, 0);

    mImage = lv_image_create(imageCell->getObject());

    if (aImageDsc != nullptr)
    {
        lv_image_set_src(mImage, aImageDsc);
        lv_obj_set_style_img_recolor(mImage, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_img_recolor_opa(mImage, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_center(mImage);
    }

    mValue = lv_label_create(valueCell->getObject());

    if (aUnit.size() > 0)
    {
        lv_label_set_text_fmt(mValue, "%d %s", aValue, aUnit.c_str());
    }
    else
    {
        lv_label_set_text_fmt(mValue, "%d", aValue);
    }

    LV_FONT_DECLARE(AlanSans_26_Medium);
    lv_obj_set_style_text_font(mValue, &AlanSans_26_Medium, LV_PART_MAIN);
    lv_obj_set_style_text_color(mValue, lv_color_white(), 0);
    lv_obj_center(mValue);
}

CInfoBox::~CInfoBox()
{
}