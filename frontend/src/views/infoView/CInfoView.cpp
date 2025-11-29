#include "CInfoView.h"
#include "CStyleFactory.h"
#include "CLogger.h"

CInfoView::CInfoView(lv_obj_t *aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight) : CObject(aParent, aWidth, aHeight, aX, aY)
{
    LOG_DEBUG("CInfoView::CInfoView() called");
    CStyleFactory::infoMain(mObject);

    lv_obj_t * label = lv_label_create(mObject);
    lv_label_set_text_fmt(label, "View : Info");
    lv_obj_center(label);
}

CInfoView::~CInfoView()
{
}

void CInfoView::callback(CCustomEventBase& aEvent)
{
    (void)aEvent;
}