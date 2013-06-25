#include "MonitorScheduler.h"

uint32_t MTR_systemTime = 0;
uint8_t  MTR_systemTimeUpdated = MTR_SYS_TIME_UPDATED;
MTR_Scheduler_State_Type MTR_Scheduler_State = MTR_Scheduler_State_Inactive;

void MTR_SysTimeHandler(void)
{
  if(MTR_systemTime >= MTR_systemTimeMax) MTR_systemTime = 0; else MTR_systemTime++;
  MTR_systemTimeUpdated = MTR_SYS_TIME_UPDATED;
}

void MTR_Scheduler(void){
  static uint32_t BackgroundFunctionIndex = 0;
  MTR_Scheduler_State = MTR_Scheduler_State_Active;

  for(; BackgroundFunctionIndex < MTR_BackgroundFunctionsListLength;BackgroundFunctionIndex++){

    (*MTR_BackgroundFunctionsList[BackgroundFunctionIndex])();

    if(MTR_systemTimeUpdated == MTR_SYS_TIME_UPDATED){
      MTR_systemTimeUpdated = MTR_SYS_TIME_NOT_UPDATED;
      MTR_Alarm_Evaluate();    /*Time trigger events are updated before they are checked*/

      if(!((MTR_systemTime + MTR_EVENT_HIGHPRIO_DELAY) % MTR_EVENT_HIGHPRIO_POLLFREQ)){
        MTR_Event_Evaluate(MTR_EVENT_HIGHPRIO);
      }

      if(!((MTR_systemTime + MTR_EVENT_NORMALPRIO_DELAY) % MTR_EVENT_NORMALPRIO_POLLFREQ)){
        MTR_Event_Evaluate(MTR_EVENT_NORMALPRIO);
      }

      if(!((MTR_systemTime + MTR_EVENT_LOWPRIO_DELAY) % MTR_EVENT_LOWPRIO_POLLFREQ)){
        MTR_Event_Evaluate(MTR_EVENT_LOWPRIO);
      }
    }
  }
  BackgroundFunctionIndex = 0;
  MTR_Scheduler_State = MTR_Scheduler_State_Inactive;
}
