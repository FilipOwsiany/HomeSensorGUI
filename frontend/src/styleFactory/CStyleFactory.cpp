#include "CStyleFactory.h"

LV_FONT_DECLARE(AlanSans_20_Medium);
LV_FONT_DECLARE(AlanSans_26_Medium);
LV_FONT_DECLARE(AlanSans_32_Medium);

// ---------------- Button ----------------

void CStyleFactory::buttonMain(lv_obj_t* mObject)
{
    if (!mObject) return;

    lv_obj_set_style_pad_hor   (mObject, 5,                 0);
    lv_obj_set_style_pad_ver   (mObject, 5,                 0);
    lv_obj_set_style_pad_row   (mObject, 0,                 0);
    lv_obj_set_style_pad_column(mObject, 0,                 0);
    lv_obj_set_style_layout    (mObject, LV_LAYOUT_GRID,    0);
}

void CStyleFactory::buttonLabel(lv_obj_t* mObject)
{
    if (!mObject) return;

    lv_obj_set_style_text_font (mObject, &AlanSans_20_Medium,   LV_PART_MAIN);
    lv_obj_set_style_text_color(mObject, lv_color_white(),      LV_PART_MAIN);
}

void CStyleFactory::buttonImage(lv_obj_t* mObject)
{
    if (!mObject) return;

    lv_obj_set_style_img_recolor    (mObject, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(mObject, LV_OPA_COVER,   LV_PART_MAIN);
}

// ---------------- InfoBox ----------------

void CStyleFactory::infoBoxMain(lv_obj_t* mObject)
{
    if (!mObject) return;

    lv_obj_set_style_bg_color       (mObject, lv_color_hex(0x0C1826), LV_PART_MAIN);
    lv_obj_set_style_bg_opa         (mObject, LV_OPA_COVER,           LV_PART_MAIN);
    lv_obj_set_style_border_width   (mObject, 0,                      LV_PART_MAIN);

    lv_obj_set_style_radius         (mObject, 8,                      LV_PART_MAIN);
    lv_obj_set_style_pad_all        (mObject, 4,                      LV_PART_MAIN);
}

void CStyleFactory::infoBoxValue(lv_obj_t* mObject)
{
    if (!mObject) return;

    lv_obj_set_style_text_font (mObject, &AlanSans_26_Medium, LV_PART_MAIN);
    lv_obj_set_style_text_color(mObject, lv_color_white(),    LV_PART_MAIN);
    lv_obj_set_style_text_align(mObject, LV_TEXT_ALIGN_LEFT,  LV_PART_MAIN);
}

void CStyleFactory::infoBoxImage(lv_obj_t* mObject)
{
    if (!mObject) return;

    lv_obj_set_style_img_recolor    (mObject, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_img_recolor_opa(mObject, LV_OPA_COVER,     LV_PART_MAIN);
}

// ---------------- Sensor ----------------

void CStyleFactory::sensorMain(lv_obj_t* mObject)
{
    if (!mObject) return;

    lv_obj_set_style_radius      (mObject, 5,                                LV_PART_MAIN);
    lv_obj_set_style_bg_opa      (mObject, LV_OPA_COVER,                     LV_PART_MAIN);
    lv_obj_set_style_bg_color    (mObject, lv_color_hex(0x102238),           LV_PART_MAIN);
    lv_obj_set_style_shadow_width(mObject, 0,                                LV_PART_MAIN);
    lv_obj_set_style_shadow_color(mObject, lv_palette_main(LV_PALETTE_RED),  LV_PART_MAIN);
    lv_obj_set_style_border_width(mObject, 0,                                LV_PART_MAIN);

    lv_obj_set_style_pad_hor   (mObject, 16, LV_PART_MAIN);
    lv_obj_set_style_pad_ver   (mObject, 16, LV_PART_MAIN);
    lv_obj_set_style_pad_row   (mObject, 16, LV_PART_MAIN);
    lv_obj_set_style_pad_column(mObject,16, LV_PART_MAIN);
}

void CStyleFactory::sensorCell(lv_obj_t* mObject)
{
    if (!mObject) return;

    lv_obj_set_style_bg_color (mObject, lv_color_hex(0x0C1826), LV_PART_MAIN);
    lv_obj_set_style_bg_opa   (mObject, LV_OPA_COVER,           LV_PART_MAIN);
}

void CStyleFactory::sensorName(lv_obj_t* mObject)
{
    if (!mObject) return;

    lv_obj_set_style_text_font  (mObject, &AlanSans_32_Medium,  LV_PART_MAIN);
    lv_obj_align                (mObject, LV_ALIGN_CENTER,      0,  0);
    lv_obj_set_style_text_color (mObject, lv_color_white(),     0);   
}

// ---------------- Sensors ----------------

void CStyleFactory::sensorsMain(lv_obj_t* mObject)
{
    lv_obj_set_style_radius        (mObject, 5,                                 0);
    lv_obj_set_style_bg_opa        (mObject, LV_OPA_COVER,                      0);
    lv_obj_set_style_bg_color      (mObject, lv_color_hex(0x0C1826),            0);
    lv_obj_set_style_shadow_width  (mObject, 0,                                 0);
    lv_obj_set_style_shadow_color  (mObject, lv_palette_main(LV_PALETTE_BLUE),  0);
    lv_obj_set_style_border_width  (mObject, 0,                                 0);

    lv_obj_set_style_pad_hor       (mObject, 16, 0);
    lv_obj_set_style_pad_ver       (mObject, 16, 0);
    lv_obj_set_style_pad_row       (mObject, 16, 0);
    lv_obj_set_style_pad_column    (mObject, 16, 0);
}

// ---------------- Settings ----------------

void CStyleFactory::settingsMain(lv_obj_t* mObject)
{
    lv_obj_set_style_radius        (mObject, 5,                                 0);
    lv_obj_set_style_bg_opa        (mObject, LV_OPA_COVER,                      0);
    lv_obj_set_style_bg_color      (mObject, lv_color_hex(0x0C1826),            0);
    lv_obj_set_style_shadow_width  (mObject, 0,                                 0);
    lv_obj_set_style_shadow_color  (mObject, lv_palette_main(LV_PALETTE_BLUE),  0);
    lv_obj_set_style_border_width  (mObject, 0,                                 0);
    lv_obj_set_style_pad_all       (mObject, 10,                                0);
}

// ---------------- Layout ----------------

void CStyleFactory::layoutMain(lv_obj_t* mObject)
{
    lv_obj_set_style_radius        (mObject, 5,                                 0);
    lv_obj_set_style_bg_opa        (mObject, LV_OPA_COVER,                      0);
    lv_obj_set_style_bg_color      (mObject, lv_color_hex(0x0C1826),            0);
    lv_obj_set_style_shadow_width  (mObject, 0,                                 0);
    lv_obj_set_style_shadow_color  (mObject, lv_palette_main(LV_PALETTE_BLUE),  0);
    lv_obj_set_style_border_width  (mObject, 0,                                 0);
    lv_obj_set_style_pad_all       (mObject, 10,                                0);
}

// ---------------- Layout ----------------

void CStyleFactory::infoMain(lv_obj_t* mObject)
{
    lv_obj_set_style_radius        (mObject, 5,                     0);
    lv_obj_set_style_bg_opa        (mObject, LV_OPA_COVER,          0);
    lv_obj_set_style_bg_color      (mObject, lv_color_hex(0x0C1826),0);
    lv_obj_set_style_shadow_width  (mObject, 0,                     0);
    lv_obj_set_style_shadow_color  (mObject, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_border_width  (mObject, 0,                     0);
    lv_obj_set_style_pad_all       (mObject, 10,                    0);
}

// ---------------- SidePanel ----------------

void CStyleFactory::sidePanelMain(lv_obj_t* mObject)
{
    lv_obj_set_style_radius       (mObject, 5,                                  0);
    lv_obj_set_style_bg_opa       (mObject, LV_OPA_COVER,                       0);
    lv_obj_set_style_bg_color     (mObject, lv_color_hex(0x0C1826),             0);
    lv_obj_set_style_shadow_width (mObject, 0,                                  0);
    lv_obj_set_style_shadow_color (mObject, lv_palette_main(LV_PALETTE_BLUE),   0);
    lv_obj_set_style_border_width (mObject, 0,                                  0);


    lv_obj_set_style_pad_hor      (mObject, 5,                                  0);
    lv_obj_set_style_pad_ver      (mObject, 5,                                  0);
    lv_obj_set_style_pad_row      (mObject, 5,                                  0);
    lv_obj_set_style_pad_column   (mObject, 0,                                  0);
}