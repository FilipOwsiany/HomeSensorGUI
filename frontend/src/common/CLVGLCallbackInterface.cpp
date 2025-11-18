#include "CLVGLCallbackInterface.h"
#include <stdio.h>


void CLVGLCallbackInterface::callback(lv_event_t * aEvent, uint32_t aId)
{
    printf("CLVGLCallbackInterface::callback called! ID: %d\n", aId);
    (void)aEvent;
    (void)aId;
}