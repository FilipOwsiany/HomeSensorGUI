#include "CSensorsView.h"

#include "CCell.h"
#include "CSensor.h"
#include "CCustomEvent.h"
#include "CStyleFactory.h"
#include "CLogger.h"

CSensorsView::CSensorsView(lv_obj_t *aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight) : CObject(aParent, aWidth, aHeight, aX, aY)
{
    LOG_DEBUG("CSensorsView::CSensorsView() called");
    CStyleFactory::sensorsMain(mObject);

    static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    
    lv_obj_set_layout(mObject, LV_LAYOUT_GRID);
    lv_obj_set_style_grid_column_dsc_array(mObject, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(mObject, row_dsc, 0);

    for(uint8_t i = 0; i < 3; i++) 
    {
        uint8_t col = i % 2;
        uint8_t row = i / 2;

        CCell* cell = new CCell(mObject, 490, 260, col, row);
        mSensors[i] = new CSensor(cell->getObject(), 0, 0, 490, 260);
    }

}

CSensorsView::~CSensorsView()
{
}

void CSensorsView::callback(CCustomEventBase& aEvent)
{
    switch (aEvent.getId())
    {
        case CCustomEventBase::EEventId::NAME:
        {
            auto sensorName = static_cast<CEventSensorName &>(aEvent);
            if (mSensors[sensorName.getSensorId()] != nullptr)
            {
                mSensors[sensorName.getSensorId()]->callback(aEvent);
            }
            break;
        }

        case CCustomEventBase::EEventId::TEMPERATURE:
        {
            auto sensorTemperature = static_cast<CEventSensorTemperature &>(aEvent);
            if (mSensors[sensorTemperature.getSensorId()] != nullptr)
            {
                mSensors[sensorTemperature.getSensorId()]->callback(aEvent);
            }
            break;
        }

        case CCustomEventBase::EEventId::HUMIDITY:
        {
            auto sensorHumidity = static_cast<CEventSensorHumidity &>(aEvent);
            if (mSensors[sensorHumidity.getSensorId()] != nullptr)
            {
                mSensors[sensorHumidity.getSensorId()]->callback(aEvent);
            }
            break;
        }

        case CCustomEventBase::EEventId::PRESSURE:
        {
            auto sensorPressure = static_cast<CEventSensorPressure &>(aEvent);
            if (mSensors[sensorPressure.getSensorId()] != nullptr)
            {
                mSensors[sensorPressure.getSensorId()]->callback(aEvent);
            }
            break;
        }

        case CCustomEventBase::EEventId::BATTERY:
        {
            auto sensorBattery = static_cast<CEventSensorBattery &>(aEvent);
            if (mSensors[sensorBattery.getSensorId()] != nullptr)
            {
                mSensors[sensorBattery.getSensorId()]->callback(aEvent);
            }
            break;
        }

        default:
            break;
    }
}