#include "CCell.h"

CCell::CCell(lv_obj_t * parent, int32_t w, int32_t h, int32_t col, int32_t row)
{
    mObject = lv_obj_create(parent);
    lv_obj_set_size(mObject, w, h);
    lv_obj_set_grid_cell(mObject,
        LV_GRID_ALIGN_STRETCH, col, 1,
        LV_GRID_ALIGN_STRETCH, row, 1);
    lv_obj_set_style_border_width(mObject, 0, 0);
    lv_obj_set_style_bg_opa(mObject, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(mObject, 0, 0);
    lv_obj_set_style_pad_row(mObject, 0, 0);
    lv_obj_set_style_pad_column(mObject, 0, 0);
    lv_obj_clear_flag(mObject, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(mObject, LV_OBJ_FLAG_CLICKABLE);
}

CCell::~CCell()
{

}
