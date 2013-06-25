#include "MonitorEvent.h"

#define NULL 0

static MTR_EventStatus MonitorEventStatus = 0;

MTR_Err MTR_Event_Set(MTR_EventMask mask)
{
  MonitorEventStatus |= mask;
  return MTR_Err_OK;
}

MTR_Err MTR_Event_Clear(MTR_EventMask mask)
{
  MonitorEventStatus &= ~mask;
  return MTR_Err_OK;
}

MTR_EventMask MTR_Event_GetStatus(void)
{
  return MonitorEventStatus;
}

MTR_Err MTR_Event_Evaluate(MTR_EventPriority prio)
{
  uint32_t i;
  uint32_t mask;
  if(prio & MTR_EVENT_HIGHPRIO){
    for(i = 0 ; i < MTR_eventStruct.MTR_EventList_HighPrio_Length ; i++){
      mask = MTR_eventStruct.MTR_EventList_HighPrio[i].mask;
      if((mask != 0) && (mask == (mask & MonitorEventStatus) ||
        ((MTR_eventStruct.MTR_EventList_HighPrio[i].relation == MTR_EVENT_RELAION_OR) && (mask & MonitorEventStatus))))
      {
        if(MTR_eventStruct.MTR_EventList_HighPrio[i].callback != NULL) MTR_eventStruct.MTR_EventList_HighPrio[i].callback();
      }
    }
  }

  if(prio & MTR_EVENT_NORMALPRIO){
    for(i = 0 ; i < MTR_eventStruct.MTR_EventList_NormalPrio_Length ; i++){
      mask = MTR_eventStruct.MTR_EventList_NormalPrio[i].mask;
      if((mask != 0) && (mask == (mask & MonitorEventStatus) ||
        ((MTR_eventStruct.MTR_EventList_NormalPrio[i].relation == MTR_EVENT_RELAION_OR) && (mask & MonitorEventStatus))))
      {
        if(MTR_eventStruct.MTR_EventList_NormalPrio[i].callback != NULL) MTR_eventStruct.MTR_EventList_NormalPrio[i].callback();
      }
    }
  }

  if(prio & MTR_EVENT_LOWPRIO){
    for(i = 0 ; i < MTR_eventStruct.MTR_EventList_LowPrio_Length ; i++){
      mask = MTR_eventStruct.MTR_EventList_LowPrio[i].mask;
      if((mask != 0) && (mask == (mask & MonitorEventStatus) ||
        ((MTR_eventStruct.MTR_EventList_LowPrio[i].relation == MTR_EVENT_RELAION_OR) && (mask & MonitorEventStatus))))
      {
        if(MTR_eventStruct.MTR_EventList_LowPrio[i].callback != NULL) MTR_eventStruct.MTR_EventList_LowPrio[i].callback();
      }
    }
  }
  return MTR_Err_OK;
}

