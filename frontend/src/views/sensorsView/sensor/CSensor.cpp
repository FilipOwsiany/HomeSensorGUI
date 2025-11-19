#include "CSensor.h"

#include "CCell.h"
#include "CCustomEvent.h"
#include "CStyleFactory.h"

CSensor::CSensor(lv_obj_t *aParent, int32_t aX, int32_t aY,
                 int32_t aWidth, int32_t aHeight)
: CObject(aParent, aWidth, aHeight, aX, aY)
{
    CStyleFactory::sensorMain(mObject);

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
                LV_GRID_ALIGN_STRETCH, 0, 2,
                LV_GRID_ALIGN_STRETCH, 0, 1);
        } 
        else 
        {
            uint8_t j   = i - 1;
            uint8_t col = j % 2;
            uint8_t row = 1 + j / 2;
            cell = new CCell(mObject, 0, 0, col, row);
        }

        CStyleFactory::sensorCell(cell->getObject());

        if (i == 0) 
        {
            mSensorName = lv_label_create(cell->getObject());
            lv_label_set_text_fmt(mSensorName, "---");
            CStyleFactory::sensorName(mSensorName);        
        }
        else
        {
            mInfoBoxes[i - 1] = new CInfoBox(
                cell->getObject(),
                0,
                0,
                LV_PCT(100),
                LV_PCT(100),
                mSensorInfoBoxNames[i - 1],
                mSensorInfoBoxImages[i - 1]
            );
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