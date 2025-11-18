#include "CSensor.h"

#include "CCell.h"
#include "CCustomEvent.h"

CSensor::CSensor(lv_obj_t *aParent, int32_t aX, int32_t aY,
                 int32_t aWidth, int32_t aHeight)
: CObject(aParent, aWidth, aHeight, aX, aY)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, 5);
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    lv_style_set_bg_color(&style, lv_color_hex(0x102238));
    lv_style_set_shadow_width(&style, 0);
    lv_style_set_shadow_color(&style, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_border_width(&style, 0);
    lv_style_set_pad_all(&style, 10);

    lv_obj_add_style(mObject, &style, 0);

    lv_obj_set_style_pad_hor(mObject, 16, 0);
    lv_obj_set_style_pad_ver(mObject, 16, 0);
    lv_obj_set_style_pad_row(mObject, 16, 0);
    lv_obj_set_style_pad_column(mObject, 16, 0);

    static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    
    lv_obj_set_layout(mObject, LV_LAYOUT_GRID);
    lv_obj_set_style_grid_column_dsc_array(mObject, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(mObject, row_dsc, 0);

    for (uint8_t i = 0; i < 5; ++i) 
    {
        CCell* cell = nullptr;
        if (i == 0) 
        {
            cell = new CCell(mObject, 0, 0, 0, 0);
            lv_obj_set_grid_cell(cell->getObject(),
                LV_GRID_ALIGN_STRETCH, 0, 2,   // kolumna 0, span 2 kolumny
                LV_GRID_ALIGN_STRETCH, 0, 1);  // wiersz 0, span 1
        } 
        else 
        {
            // reszta: wiersze zaczynają się od 1 (pod szerokim kaflem)
            uint8_t j   = i - 1;
            uint8_t col = j % 2;
            uint8_t row = 1 + j / 2;           // 1: start od drugiego wiersza
            cell = new CCell(mObject, 0, 0, col, row);
        }

        lv_obj_set_style_bg_color(cell->getObject(), lv_color_hex(0x0C1826), 0);
        lv_obj_set_style_bg_opa(cell->getObject(), LV_OPA_COVER, 0);   // 255;

        if (i == 0) 
        {
            mSensorName = lv_label_create(cell->getObject());
            lv_label_set_text_fmt(mSensorName, "---");
            LV_FONT_DECLARE(AlanSans_32_Medium);
            lv_obj_set_style_text_font(mSensorName, &AlanSans_32_Medium, LV_PART_MAIN);
            lv_obj_align(mSensorName, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_color(cell->getObject(), lv_color_white(), 0);          
        }
        else
        {
            lv_obj_set_style_pad_hor(cell->getObject(), 5, 0);
            lv_obj_set_style_pad_ver(cell->getObject(), 5, 0);
            lv_obj_set_style_pad_row(cell->getObject(), 5, 0);
            lv_obj_set_style_pad_column(cell->getObject(), 5, 0);

            mInfoBoxes[i - 1] = new CInfoBox(
                cell->getObject(),
                0,
                0,
                LV_PCT(100),
                LV_PCT(100),
                mSensorInfoBoxNames[i - 1],
                mSensorInfoBoxImages[i - 1]
            );

            // CObject* obj = new CObject(cell->getObject(), 0, 0, 0, 0);
            // lv_obj_set_size(obj->getObject(), LV_PCT(100), LV_PCT(100));
            // lv_obj_set_style_bg_color(obj->getObject(), lv_color_hex(0x0C1826), 0);
            // lv_obj_set_style_bg_opa(obj->getObject(), LV_OPA_COVER, 0);   // 255;
            // lv_obj_set_style_border_width(obj->getObject(), 0, 0);

            // static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};
            // static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
            
            // lv_obj_set_layout(obj->getObject(), LV_LAYOUT_GRID);
            // lv_obj_set_style_grid_column_dsc_array(obj->getObject(), col_dsc, 0);
            // lv_obj_set_style_grid_row_dsc_array(obj->getObject(), row_dsc, 0);

            // CCell* cell1 = new CCell(obj->getObject(), 0, 0, 0, 0);
            // CCell* cell2 = new CCell(obj->getObject(), 0, 0, 1, 0);

            // lv_obj_t * mImage = lv_image_create(cell1->getObject());
            // lv_image_set_src(mImage, mSensorInfoBoxImages[i - 1]);
            // //lv_image_set_scale(mImage, 16); // 50%
            // lv_obj_set_style_img_recolor(mImage, lv_color_white(), LV_PART_MAIN);
            // lv_obj_set_style_img_recolor_opa(mImage, LV_OPA_COVER, LV_PART_MAIN); // 255 = pełny tint
            // lv_obj_center(mImage);

            // lv_obj_t * mLabel = lv_label_create(cell2->getObject());
            // lv_label_set_text_fmt(mLabel, "%s", "12.3");

            // LV_FONT_DECLARE(AlanSans_26_Medium);
            // lv_obj_set_style_text_font(mLabel, &AlanSans_26_Medium, LV_PART_MAIN);
            // lv_obj_set_style_text_color(mLabel, lv_color_white(), 0);
            // lv_obj_center(mLabel);

            // infoBox->setValue(12 + i);

        }
    }
}

CSensor::~CSensor()
{

}

void CSensor::callback(CCustomEventBase& aEvent)
{
    switch (aEvent.getId())
    {
        case CCustomEventBase::EEventId::NAME:
        {
            auto sensorName = static_cast<CEventSensorName &>(aEvent);
            lv_label_set_text_fmt(mSensorName, "%s", sensorName.getSensorName().c_str());
            break;
        }

        case CCustomEventBase::EEventId::TEMPERATURE:
        {
            auto sensorTemperature = static_cast<CEventSensorTemperature &>(aEvent);
            mInfoBoxes[static_cast<uint32_t>(ESensorInfoBoxIds::eTemperature)]->setValue(sensorTemperature.getSensorTemperature());
            break;
        }

        case CCustomEventBase::EEventId::HUMIDITY:
        {
            auto sensorHumidity = static_cast<CEventSensorHumidity &>(aEvent);
            mInfoBoxes[static_cast<uint32_t>(ESensorInfoBoxIds::eHumidity)]->setValue(sensorHumidity.getSensorHumidity());
            break;
        }

        case CCustomEventBase::EEventId::PRESSURE:
        {
            auto sensorPressure = static_cast<CEventSensorPressure &>(aEvent);
            mInfoBoxes[static_cast<uint32_t>(ESensorInfoBoxIds::ePressure)]->setValue(sensorPressure.getSensorPressure());
            break;
        }

        case CCustomEventBase::EEventId::BATTERY:
        {
            auto sensorBattery = static_cast<CEventSensorBattery &>(aEvent);
            mInfoBoxes[static_cast<uint32_t>(ESensorInfoBoxIds::eBattery)]->setValue(sensorBattery.getSensorBatteryLevel());
            break;
        }

        default:
            break;
    }
}