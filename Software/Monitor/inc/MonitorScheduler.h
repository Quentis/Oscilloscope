#ifndef MONITORSCHEDULER_H_
#define MONITORSCHEDULER_H_

#include "MonitorCfg.h"
#include "MonitorEvent.h"
#include "MonitorAlarm.h"
#include "MonitorTypes.h"
#include "stdint.h"

#define MTR_SYS_TIME_NOT_UPDATED    0
#define MTR_SYS_TIME_UPDATED        (!MTR_SYS_TIME_NOT_UPDATED)

extern const uint32_t MTR_systemTimeMax;
extern void (*MTR_BackgroundFunctionsList[])(void);
extern uint32_t MTR_BackgroundFunctionsListLength;

void MTR_Scheduler(void);

#endif /* MONITORSCHEDULER_H_ */
