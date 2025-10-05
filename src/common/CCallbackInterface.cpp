#include "CCallbackInterface.h"
#include <stdio.h>


void CCallbackInterface::callback(lv_event_t * aEvent, uint32_t aId)
{
    printf("CCallbackInterface::callback called! ID: %d\n", aId);
    (void)aEvent;
    (void)aId;
}