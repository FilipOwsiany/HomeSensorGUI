#ifndef C_CELL_H
#define C_CELL_H

#include <stdio.h>
#include <vector>
#include <algorithm>

#include "lvgl/lvgl.h"

#include "CObject.h"

class CCell : public CObject
{
private:
public:
    CCell(lv_obj_t * aParent, int32_t aWidth, int32_t aHeight, int32_t aCol, int32_t aRow);
    ~CCell();
};

#endif // C_CELL_H