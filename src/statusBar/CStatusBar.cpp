#include "CStatusBar.h"

#include "CButton.h"
#include "CCell.h"
#include "CCustomEvent.h"

CStatusBar::CStatusBar(lv_obj_t *aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight)
{

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, 5);
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    lv_style_set_bg_color(&style, lv_color_hex(0x0C1826));
    lv_style_set_shadow_width(&style, 0);
    lv_style_set_shadow_color(&style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&style, 0);
    lv_style_set_pad_all(&style, 10);

    mObject = lv_obj_create(aParent);
    lv_obj_set_size(mObject, aWidth, aHeight);
    lv_obj_set_pos(mObject, aX, aY);
    lv_obj_update_layout(mObject);

    lv_obj_add_style(mObject, &style, 0);

    static int32_t col_dsc[] = {70, 70, 70, 70, LV_GRID_FR(1), 340, LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {
        LV_GRID_FR(1),
        LV_GRID_TEMPLATE_LAST};

    lv_obj_set_style_grid_column_dsc_array(mObject, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(mObject, row_dsc, 0);
    lv_obj_set_style_pad_hor(mObject, 5, 0);
    lv_obj_set_style_pad_ver(mObject, 5, 0);
    lv_obj_set_style_pad_row(mObject, 5, 0);
    lv_obj_set_style_pad_column(mObject, 0, 0);
    lv_obj_set_layout(mObject, LV_LAYOUT_GRID);

    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t col = i;
        if (i == 4)
        {
            col = 5;
        }

        CCell *cell = nullptr;
        if (i == 4)
        {
            /* code */
            cell = new CCell(mObject, 310, lv_obj_get_height(mObject) - 10, col, 0);
        }
        else
        {
            /* code */
            cell = new CCell(mObject, 60, lv_obj_get_height(mObject) - 10, col, 0);
        }

        // CButton* button = new CButton(i, *this, cell->getObject(), 0, 0, 150, 80, mSidePanelButtonNames[i]);
        // mButtonContainers.push_back({button, i, mSidePanelButtonNames[i]});
        // button->addImage(mSidePanelButtonImages[i]);

        if (i < 4)
        {
            /* code */
            lv_obj_t *img = lv_img_create(cell->getObject());
            lv_image_set_src(img, mStatusbarIconImages[i]);
            lv_obj_set_style_img_recolor(img, lv_color_white(), LV_PART_MAIN);
            lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, LV_PART_MAIN);
            lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
            mIconImagesObjects[i] = img;
            // mIconImagesContainers.push_back({img, static_cast<uint32_t>(i), mStatusbarIconImageNames[i]});
        }

        if (i == 4)
        {
            lv_obj_t *label = lv_label_create(cell->getObject());
            lv_label_set_text_fmt(label, "1.10.2025 12:45:30");
            LV_FONT_DECLARE(AlanSans_32_Medium);
            lv_obj_set_style_text_font(label, &AlanSans_32_Medium, LV_PART_MAIN);
            lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_color(cell->getObject(), lv_color_white(), 0);
        }
    }
}

CStatusBar::~CStatusBar()
{
}

void CStatusBar::callback(CCustomEventBase &aEvent)
{
    printf("CStatusBar::callback called! ID: %d\n", aEvent.getId());

    // uint32_t eventId = static_cast<uint32_t>(aEvent.getId());

    // auto itIconImage = std::find_if(
    //     mIconImagesContainers.begin(), mIconImagesContainers.end(),
    //     [eventId](const SObjectContairner<lv_obj_t>& c) {
    //         return c.mId == eventId;
    //     });

    // lv_obj_t * img = nullptr;

    // if (itIconImage != mIconImagesContainers.end())
    // {
    //     img = itIconImage->mObject;
    // }

    switch (aEvent.getId())
    {
    case CCustomEventBase::EEventId::WIFI:
    {
        lv_obj_t *img = mIconImagesObjects[static_cast<uint32_t>(EIconImageIds::eWifi)];
        if (img == nullptr)
        {
            break;
        }

        auto wifiEvent = static_cast<CEventWifi &>(aEvent);

        if (wifiEvent.getWifiEventType() == CEventWifi::EEventWifiType::CONNECTED)
        {
            lv_obj_set_style_img_recolor(img, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
            lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, LV_PART_MAIN);
            printf("CStatusBar::callback WIFI CONNECTED\n");
        }
        else if (wifiEvent.getWifiEventType() == CEventWifi::EEventWifiType::DISCONNECTED)
        {
            lv_obj_set_style_img_recolor(img, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
            lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, LV_PART_MAIN);
            printf("CStatusBar::callback WIFI DISCONNECTED\n");
        }
        else if (wifiEvent.getWifiEventType() == CEventWifi::EEventWifiType::GOT_IP)
        {
            /* code */
            printf("CStatusBar::callback WIFI GOT_IP\n");
        }
        else if (wifiEvent.getWifiEventType() == CEventWifi::EEventWifiType::LOST_IP)
        {
            /* code */
            printf("CStatusBar::callback WIFI LOST_IP\n");
        }
        break;
    }
    case CCustomEventBase::EEventId::CLOUD:
    {
        lv_obj_t *img = mIconImagesObjects[static_cast<uint32_t>(EIconImageIds::eCloud)];
        if (img == nullptr)
        {
            break;
        }

        auto cloudEvent = static_cast<CEventCloud &>(aEvent);

        if (cloudEvent.getCloudEventType() == CEventCloud::EEventCloudType::CONNECTED)
        {
            lv_obj_set_style_img_recolor(img, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
            lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, LV_PART_MAIN);
            printf("CStatusBar::callback CLOUD CONNECTED\n");
        }
        else if (cloudEvent.getCloudEventType() == CEventCloud::EEventCloudType::DISCONNECTED)
        {
            lv_obj_set_style_img_recolor(img, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
            lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, LV_PART_MAIN);
            printf("CStatusBar::callback CLOUD DISCONNECTED\n");
        }
        else if (cloudEvent.getCloudEventType() == CEventCloud::EEventCloudType::DATA_SENT)
        {
            /* code */
            printf("CStatusBar::callback CLOUD DATA_SENT\n");
        }
        else if (cloudEvent.getCloudEventType() == CEventCloud::EEventCloudType::DATA_RECEIVED)
        {
            /* code */
            printf("CStatusBar::callback CLOUD DATA_RECEIVED\n");
        }
        break;
    }
    case CCustomEventBase::EEventId::BLUETOOTH:
    {
        lv_obj_t *img = mIconImagesObjects[static_cast<uint32_t>(EIconImageIds::eBluetooth)];
        if (img == nullptr)
        {
            break;
        }

        auto bluetoothEvent = static_cast<CEventBluetooth &>(aEvent);

        if (bluetoothEvent.getBluetoothEventType() == CEventBluetooth::EEventBluetoothType::CONNECTED)
        {
            lv_obj_set_style_img_recolor(img, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
            lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, LV_PART_MAIN);
            printf("CStatusBar::callback BLUETOOTH CONNECTED\n");
        }
        else if (bluetoothEvent.getBluetoothEventType() == CEventBluetooth::EEventBluetoothType::DISCONNECTED)
        {
            lv_obj_set_style_img_recolor(img, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
            lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, LV_PART_MAIN);
            printf("CStatusBar::callback BLUETOOTH DISCONNECTED\n");
        }
        else if (bluetoothEvent.getBluetoothEventType() == CEventBluetooth::EEventBluetoothType::DATA_SENT)
        {
            /* code */
            printf("CStatusBar::callback BLUETOOTH DATA_SENT\n");
        }
        else if (bluetoothEvent.getBluetoothEventType() == CEventBluetooth::EEventBluetoothType::DATA_RECEIVED)
        {
            /* code */
            printf("CStatusBar::callback BLUETOOTH DATA_RECEIVED\n");
        }
        break;
    }
    default:
        break;
    }

    delete &aEvent;
}