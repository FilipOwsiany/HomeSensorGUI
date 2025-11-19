#include "CSettingsView.h"
#include "CStyleFactory.h"

CSettingsView::CSettingsView(lv_obj_t *aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight) : CObject(aParent, aWidth, aHeight, aX, aY)
{
    CStyleFactory::settingsMain(mObject);

    lv_obj_t * label = lv_label_create(mObject);
    lv_label_set_text_fmt(label, "View : Settings");
    lv_obj_center(label);
}

CSettingsView::~CSettingsView()
{
}

void CSettingsView::callback(CCustomEventBase& aEvent)
{
    (void)aEvent;
}