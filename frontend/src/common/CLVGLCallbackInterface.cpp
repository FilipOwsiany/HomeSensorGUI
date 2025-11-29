#include "CLVGLCallbackInterface.h"
#include "CLogger.h"
#include <stdio.h>


void CLVGLCallbackInterface::callback(lv_event_t * aEvent, uint32_t aId)
{
    LOGF_WARN("CLVGLCallbackInterface::callback() called but not implemented! ID: %u\n", aId);
    (void)aEvent;
    (void)aId;
}