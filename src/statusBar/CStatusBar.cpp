#include "CStatusBar.h"

#include "CButton.h"
#include "CCell.h"

CStatusBar::CStatusBar(lv_obj_t * aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight)
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
        LV_GRID_TEMPLATE_LAST
    };

    lv_obj_set_style_grid_column_dsc_array(mObject, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(mObject, row_dsc, 0);
    lv_obj_set_style_pad_hor(mObject, 5, 0);
    lv_obj_set_style_pad_ver(mObject, 5, 0);
    lv_obj_set_style_pad_row(mObject, 5, 0);
    lv_obj_set_style_pad_column(mObject, 0, 0);
    lv_obj_set_layout(mObject, LV_LAYOUT_GRID);

    for(uint8_t i = 0; i < 5; i++) 
    {
        uint8_t col = i;
        if (i == 4)
        {
            col = 5;
        }
        
        CCell* cell = nullptr;
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
            lv_obj_t * img = lv_img_create(cell->getObject());
            lv_image_set_src(img, mStatusbarIconImages[i]);
            lv_obj_set_style_img_recolor(img, lv_color_white(), LV_PART_MAIN);
            lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, LV_PART_MAIN);
            lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
        }
        


        if (i == 4)
        {
            lv_obj_t * label = lv_label_create(cell->getObject());
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