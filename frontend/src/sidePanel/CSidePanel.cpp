#include "CSidePanel.h"
#include "CButton.h"
#include "CCell.h"
#include "CStyleFactory.h"
#include "CLogger.h"
#include <algorithm>

CSidePanel::CSidePanel(CLVGLCallbackInterface& aCallbackInterface,
                       lv_obj_t* aParent,
                       int32_t aX,
                       int32_t aY,
                       int32_t aWidth,
                       int32_t aHeight)
    : itsCallbackInterface(aCallbackInterface)
{
    LOG_DEBUG("CSidePanel::CSidePanel() called");
    if (!aParent) return;

    mObject = lv_obj_create(aParent);

    setPosition(aX, aY);
    setSize(aWidth, aHeight);

    CStyleFactory::sidePanelMain(mObject);

    lv_obj_clear_flag(mObject, LV_OBJ_FLAG_SCROLLABLE);

    static int32_t col_dsc[] = { LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };
    static int32_t row_dsc[] = { 100, 100, 100, 100, LV_GRID_FR(1), 100, LV_GRID_TEMPLATE_LAST };

    lv_obj_set_style_grid_column_dsc_array(mObject, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(mObject, row_dsc, 0);

    lv_obj_set_layout(mObject, LV_LAYOUT_GRID);

    for (uint8_t i = 0; i < static_cast<uint8_t>(ESidePanelButtonIds::eButtonCount); i++) {
        uint8_t row = (i == static_cast<uint8_t>(ESidePanelButtonIds::eBack)) ? 5 : i;
        CCell* cell = new CCell(mObject, 160, 90, 0, row);
        CButton* button = new CButton(
            i,
            *this,
            cell->getObject(),
            0,
            0,
            150,
            80,
            mSidePanelButtonNames[i],
            mSidePanelButtonImages[i]
        );
        mButtonContainers.push_back({ button, i, mSidePanelButtonNames[i] });
    }
}

CSidePanel::~CSidePanel() {}

void CSidePanel::callback(lv_event_t* aEvent, uint32_t aId)
{
    LOGF_DEBUG("CSidePanel::callback() called with ID: %u\n", aId);

    static lv_style_t grad_style;
    lv_style_init(&grad_style);

    static lv_grad_dsc_t grad;
    lv_color_t colors[2] = { lv_color_hex(0x6B62F1), lv_color_hex(0xA29BFE) };
    lv_opa_t opa[2] = { LV_OPA_COVER, LV_OPA_COVER };
    uint8_t fracs[2] = { 0, 255 };
    lv_grad_init_stops(&grad, colors, opa, fracs, 2);
    lv_grad_linear_init(&grad, LV_GRAD_LEFT, LV_GRAD_TOP, LV_GRAD_RIGHT, LV_GRAD_BOTTOM, LV_GRAD_EXTEND_PAD);
    lv_style_set_bg_grad(&grad_style, &grad);

    auto itButton = std::find_if(mButtonContainers.begin(), mButtonContainers.end(),
                                 [aId](const SObjectContairner<CButton>& c) { return c.mId == aId; });

    if (itButton != mButtonContainers.end()) {
        for (auto& container : mButtonContainers) {
            if (container.mId == aId) {
                lv_obj_set_style_bg_color(container.mObject->getObject(), lv_color_hex(0x6B62F1), 0);
                lv_obj_add_style(container.mObject->getObject(), &grad_style, 0);
                lv_obj_set_style_img_recolor(container.mObject->getImage(), lv_color_black(), LV_PART_MAIN);
                lv_obj_set_style_img_recolor_opa(container.mObject->getImage(), LV_OPA_COVER, LV_PART_MAIN);
                lv_obj_set_style_text_color(container.mObject->getLabel(), lv_color_black(), 0);
            } else {
                lv_obj_remove_style(container.mObject->getObject(), &grad_style, 0);
                lv_obj_set_style_bg_color(container.mObject->getObject(), lv_color_hex(0x102238), 0);
                lv_obj_set_style_img_recolor(container.mObject->getImage(), lv_color_white(), LV_PART_MAIN);
                lv_obj_set_style_img_recolor_opa(container.mObject->getImage(), LV_OPA_COVER, LV_PART_MAIN);
                lv_obj_set_style_text_color(container.mObject->getLabel(), lv_color_white(), 0);
            }
        }
    }

    itsCallbackInterface.callback(aEvent, aId);
}
