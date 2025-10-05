#include "CSidePanel.h"

#include "CButton.h"
#include "CCell.h"

CSidePanel::CSidePanel(CCallbackInterface& aCallbackInterface, lv_obj_t * aParent, int32_t aX, int32_t aY, int32_t aWidth, int32_t aHeight)
    : itsCallbackInterface(aCallbackInterface)
{
    if (aParent == nullptr)
    {
        return;
    }

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
    lv_obj_add_style(mObject, &style, 0);
    setPosition(aX, aY);
    setSize(aWidth, aHeight);
    lv_obj_clear_flag(mObject, LV_OBJ_FLAG_SCROLLABLE);

    static int32_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {
        100,            // Home
        100,            // Layout
        100,            // Settings
        100,            // Info
        LV_GRID_FR(1),  // ---
        100,            // Back
        LV_GRID_TEMPLATE_LAST
    };

    lv_obj_set_style_grid_column_dsc_array(mObject, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(mObject, row_dsc, 0);
    lv_obj_set_style_pad_hor(mObject, 5, 0);
    lv_obj_set_style_pad_ver(mObject, 5, 0);
    lv_obj_set_style_pad_row(mObject, 5, 0);
    lv_obj_set_style_pad_column(mObject, 0, 0);
    lv_obj_set_layout(mObject, LV_LAYOUT_GRID);


    for(uint8_t i = 0; i < static_cast<uint8_t>(ESidePanelButtonIds::eButtonCount); i++) 
    {
        uint8_t row = i;
        if (i == static_cast<uint8_t>(ESidePanelButtonIds::eBack)) 
        {
            row = 5;
        }
        CCell* cell = new CCell(mObject, 160, 90, 0, row);
        CButton* button = new CButton(i, *this, cell->getObject(), 0, 0, 150, 80, mSidePanelButtonNames[i]);
        mButtonContainers.push_back({button, i, mSidePanelButtonNames[i]});
        button->addImage(mSidePanelButtonImages[i]);
    }
}

CSidePanel::~CSidePanel()
{

}

void CSidePanel::callback(lv_event_t * aEvent, uint32_t aId)
{
    printf("CSidePanel::callback called! ID: %d\n", aId);

// utwórz styl tymczasowy
static lv_style_t grad_style;
lv_style_init(&grad_style);

// utwórz deskryptor gradientu
static lv_grad_dsc_t grad;

// dwa kolory
lv_color_t colors[2] = { lv_color_hex(0x6B62F1), lv_color_hex(0xA29BFE) };
lv_opa_t opa[2]      = { LV_OPA_COVER, LV_OPA_COVER };
uint8_t fracs[2]     = { 0, 255 };

// zainicjalizuj stopnie gradientu
lv_grad_init_stops(&grad, colors, opa, fracs, 2);

// ustaw liniowy gradient od (0,0) do (100%,100%) → ukośny
lv_grad_linear_init(&grad,
                    LV_GRAD_LEFT, LV_GRAD_TOP,      // start
                    LV_GRAD_RIGHT, LV_GRAD_BOTTOM,  // end
                    LV_GRAD_EXTEND_PAD);

// przypisz gradient do stylu
lv_style_set_bg_grad(&grad_style, &grad);

    auto itButton = std::find_if(mButtonContainers.begin(), mButtonContainers.end(),
                        [aId](const SObjectContairner<CButton>& c){
                            return c.mId == aId;
                        });

    if (itButton != mButtonContainers.end()) 
    {
        for (auto& container : mButtonContainers) 
        {
            lv_obj_t * btn = static_cast<lv_obj_t *>(lv_event_get_target(aEvent));
            if (container.mId == aId) 
            {
                lv_obj_set_style_bg_color(container.mObject->getObject(), lv_color_hex(0x6B62F1), 0);     // kolor startowy
                // lv_obj_set_style_bg_grad_color(container.mObject->getObject(), lv_color_hex(0xA29BFE), 0); // kolor końcowy
                // lv_obj_set_style_bg_grad_dir(container.mObject->getObject(), LV_GRAD_DIR_RADIAL, 0);          // pionowy gradient


// // utwórz styl tymczasowy
// static lv_style_t grad_style;
// lv_style_init(&grad_style);

// // utwórz deskryptor gradientu
// static lv_grad_dsc_t grad;

// // dwa kolory
// lv_color_t colors[2] = { lv_color_hex(0x6B62F1), lv_color_hex(0xA29BFE) };
// lv_opa_t opa[2]      = { LV_OPA_COVER, LV_OPA_COVER };
// uint8_t fracs[2]     = { 0, 255 };

// // zainicjalizuj stopnie gradientu
// lv_grad_init_stops(&grad, colors, opa, fracs, 2);

// // ustaw liniowy gradient od (0,0) do (100%,100%) → ukośny
// lv_grad_linear_init(&grad,
//                     LV_GRAD_LEFT, LV_GRAD_TOP,      // start
//                     LV_GRAD_RIGHT, LV_GRAD_BOTTOM,  // end
//                     LV_GRAD_EXTEND_PAD);

// // przypisz gradient do stylu
// lv_style_set_bg_grad(&grad_style, &grad);

// nałóż styl na obiekt
                lv_obj_add_style(container.mObject->getObject(), &grad_style, 0);

                lv_obj_set_style_img_recolor(container.mObject->getImage(), lv_color_black(), LV_PART_MAIN);
                lv_obj_set_style_img_recolor_opa(container.mObject->getImage(), LV_OPA_COVER, LV_PART_MAIN);
                lv_obj_set_style_text_color(container.mObject->getLabel(), lv_color_black(), 0);
            }
            else 
            {
lv_obj_remove_style(container.mObject->getObject(), &grad_style, 0);

// i ustawić jednolity kolor
lv_obj_set_style_bg_color(container.mObject->getObject(), lv_color_hex(0x102238), 0);


                lv_obj_set_style_img_recolor(container.mObject->getImage(), lv_color_white(), LV_PART_MAIN);
                lv_obj_set_style_img_recolor_opa(container.mObject->getImage(), LV_OPA_COVER, LV_PART_MAIN);
                lv_obj_set_style_text_color(container.mObject->getLabel(), lv_color_white(), 0);
            }
        }
    }
    itsCallbackInterface.callback(aEvent, aId);
}