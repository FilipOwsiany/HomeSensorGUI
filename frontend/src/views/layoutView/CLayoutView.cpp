#include "CLayoutView.h"
#include "CStyleFactory.h"
#include "CLogger.h"

CLayoutView::CLayoutView(lv_obj_t *aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight) : CObject(aParent, aWidth, aHeight, aX, aY)
{
    LOG_DEBUG("CLayoutView::CLayoutView() called");
    CStyleFactory::layoutMain(mObject);

    lv_obj_t * label = lv_label_create(mObject);
    lv_label_set_text_fmt(label, "View : Layout");
    lv_obj_center(label);
}

CLayoutView::~CLayoutView()
{
}

void CLayoutView::callback(CCustomEventBase& aEvent)
{
    (void)aEvent;
}