#include "CSettingsView.h"

CSettingsView::CSettingsView(lv_obj_t *aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight) : CObject(aParent, aWidth, aHeight, aX, aY)
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

    lv_obj_t * label = lv_label_create(mObject);
    lv_obj_add_style(mObject, &style, 0);
    lv_label_set_text_fmt(label, "View : Settings");
    lv_obj_center(label);
}

CSettingsView::~CSettingsView()
{
}

void CSettingsView::callback(CEventBase& aEvent)
{
    (void)aEvent;
}