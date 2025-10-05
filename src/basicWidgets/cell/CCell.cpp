#include "CCell.h"

#define DEBUG_CELL 0

CCell::CCell(lv_obj_t * aParent, int32_t aWidth, int32_t aHeight, int32_t aCol, int32_t aRow)
{
    mObject = lv_obj_create(aParent);
    lv_obj_set_size(mObject, aWidth, aHeight);
    lv_obj_set_style_radius(mObject, 0, 0);
    lv_obj_clear_flag(mObject, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(mObject, LV_OBJ_FLAG_CLICKABLE); 
    lv_obj_set_grid_cell(mObject,
        LV_GRID_ALIGN_CENTER, aCol, 1,
        LV_GRID_ALIGN_CENTER, aRow, 1);

    lv_obj_set_style_bg_opa(mObject, LV_OPA_TRANSP, 0);
#if DEBUG_CELL == 1
    lv_obj_set_style_border_width(mObject, 1, 0);
    lv_obj_set_style_border_color(mObject, lv_palette_main(LV_PALETTE_RED), 0);    
#else
    lv_obj_set_style_border_width(mObject, 0, 0);
#endif
}

CCell::~CCell()
{

}
