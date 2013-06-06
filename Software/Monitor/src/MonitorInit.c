#include "MonitorInit.h"

void MTR_Init(void){
  uint32_t i = 0;
  MTR_HardwareInit();

  for (i = 0; i < MTR_AlarmStruct.length; ++i) {
    MTR_AlarmStruct.list[i].status = MTR_ALARM_STATUS_DISABLE;
  }

  for (i = 0; i < MTR_InitFuncListLength; i++) {
    (*MTR_InitFuncList[i])();
  }
}
