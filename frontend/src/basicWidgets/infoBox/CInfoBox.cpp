#include "CInfoBox.h"
#include "CStyleFactory.h"

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
    CStyleFactory::infoBoxMain(mObject);

    static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    
    lv_obj_set_layout(mObject, LV_LAYOUT_GRID);
    lv_obj_set_style_grid_column_dsc_array(mObject, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(mObject, row_dsc, 0);

    CCell* imageCell = new CCell(mObject, 0, 0, 0, 0);
    CCell* valueCell = new CCell(mObject, 0, 0, 1, 0);

    mImage = lv_image_create(imageCell->getObject());
    CStyleFactory::infoBoxImage(mObject);

    if (aImageDsc != nullptr)
    {
        lv_image_set_src(mImage, aImageDsc);
        lv_obj_set_style_img_recolor(mImage, lv_color_white(), LV_PART_MAIN);
        lv_obj_set_style_img_recolor_opa(mImage, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_center(mImage);
    }

    mValue = lv_label_create(valueCell->getObject());
    CStyleFactory::infoBoxValue(valueCell->getObject());

    if (aUnit.size() > 0)
    {
        lv_label_set_text_fmt(mValue, "%.2f %s", static_cast<float>(aValue / 1000.0f), aUnit.c_str());
    }
    else
    {
        lv_label_set_text_fmt(mValue, "%.2f", static_cast<float>(aValue / 1000.0f));
    }

    lv_obj_center(mValue);
}

CInfoBox::~CInfoBox()
{
}

void CInfoBox::addImage(const lv_img_dsc_t* aImageDsc)
{
    if (mImage != nullptr)
    {
        lv_image_set_src(mImage, aImageDsc);
    }
}

void CInfoBox::setValue(uint32_t aValue)
{
    mValueData = aValue;
    if (mValue != nullptr)
    {
        if (mUnit.size() > 0)
        {
            lv_label_set_text_fmt(mValue, "%.2f %s", static_cast<float>(aValue / 1000.0f), mUnit.c_str());
        }
        else
        {
            lv_label_set_text_fmt(mValue, "%.2f", static_cast<float>(aValue / 1000.0f));
        }
    }
}
