#include "MonitorAlarm.h"

MTR_Err MTR_Alarm_QueueSort(void){
  uint32_t i,j,min,min_pos,timeLeft;
  MTR_Alarm* temp;
  MTR_AlarmPtr* ptrList = MTR_AlarmStruct.ptrList;

  for(i = 0; i < MTR_AlarmStruct.length - 1 ; i++){
      min = ~0;
      min_pos = i;

      for(j = i; j < MTR_AlarmStruct.length ; j++){
        timeLeft = ((ptrList[j]->triggerTime < MTR_systemTime)?
                   (MTR_systemTimeMax - MTR_systemTime + ptrList[j]->triggerTime):
                   (ptrList[j]->triggerTime - MTR_systemTime));
        if(timeLeft < min){
          min = timeLeft;
          min_pos = j;
        }
      }

      if(min_pos != i){
        temp = ptrList[min_pos];
        ptrList[min_pos] = ptrList[i];
        ptrList[i] = temp;
      }

  }
  return MTR_Err_OK;
}

MTR_Err MTR_Alarm_Evaluate(void){
  uint32_t i;
  MTR_AlarmPtr* ptrList = MTR_AlarmStruct.ptrList;
  for(i = 0; (ptrList[i]->triggerTime == MTR_systemTime) && (i < MTR_AlarmStruct.length); i++){
    if(ptrList[i]->status == MTR_ALARM_STATUS_ENABLE){
      if(ptrList[i]->eventMask) MTR_Event_Set(ptrList[i]->eventMask);
      if(ptrList[i]->callback != NULL){
        ptrList[i]->callback();
      }
      if(ptrList[i]->period != MTR_ALARM_PERIOD_SINGLE_EVENT){
        ptrList[i]->triggerTime = (ptrList[i]->triggerTime + ptrList[i]->period) % MTR_systemTimeMax;
      } else {
        ptrList[i]->status = MTR_ALARM_STATUS_DISABLE;
      }
    }
  }
  MTR_Alarm_QueueSort();
  return MTR_Err_OK;
}

MTR_Err MTR_Alarm_Configure(MTR_AlarmIndex targetAlarmIndex,MTR_AlarmConfiguration* alarmConfig){
  /*first parameter must be an element of the alarm list global variable*/
  MTR_Alarm* targetAlarm = &MTR_AlarmStruct.list[targetAlarmIndex];
  if(targetAlarm->status == MTR_ALARM_STATUS_ENABLE) return MTR_Err_ObjectLocked;
  else if(alarmConfig->delay >= MTR_systemTimeMax || alarmConfig->period >= MTR_systemTimeMax) return MTR_Err_InvalidParam;
  else{
    targetAlarm->delay      =   alarmConfig->delay;
    targetAlarm->period     =   alarmConfig->period;
    targetAlarm->eventMask  =   alarmConfig->eventMask;
    targetAlarm->callback   =   alarmConfig->callback;
  }
  return MTR_Err_OK;
}

MTR_Err MTR_Alarm_Enable(MTR_AlarmIndex targetAlarmIndex){
  MTR_Alarm* targetAlarm = &MTR_AlarmStruct.list[targetAlarmIndex];
  if(targetAlarm->status == MTR_ALARM_STATUS_DISABLE){
    targetAlarm->triggerTime = (MTR_systemTime + (targetAlarm->delay ? targetAlarm->delay : targetAlarm->period)) % MTR_systemTimeMax;
    targetAlarm->status = MTR_ALARM_STATUS_ENABLE;
    MTR_Alarm_QueueSort();
    return MTR_Err_OK;
  } else {
    return MTR_Err_ReStart;
  }
}

MTR_Err MTR_Alarm_Disable(MTR_AlarmIndex targetAlarmIndex){
  MTR_Alarm* targetAlarm = &MTR_AlarmStruct.list[targetAlarmIndex];
  if(targetAlarm->status == MTR_ALARM_STATUS_ENABLE){
    targetAlarm->status = MTR_ALARM_STATUS_DISABLE;
    return MTR_Err_OK;
  } else {
    return MTR_Err_ReStop;
  }
}


