#ifndef S_OBJECT_CONTAIRNER_H
#define S_OBJECT_CONTAIRNER_H

#include "lvgl/lvgl.h"
#include <string>

template<typename T>
struct SObjectContairner
{
    T * mObject = nullptr;
    uint32_t mId = 0;
    std::string mName;
};

#endif // S_OBJECT_CONTAIRNER_H