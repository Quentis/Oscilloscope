#ifndef MONITORTIMETRIGGER_H_
#define MONITORTIMETRIGGER_H_

#include "stdint.h"
#include "MonitorTypes.h"
#include "MonitorCfg.h"
#include "MonitorHardwareAbstraction.h"

#define  MTR_ALARM_EVENTMASK_NOMASK     0
#define  MTR_ALARM_STATUS_DISABLE       0
#define  MTR_ALARM_STATUS_ENABLE        (!MTR_ALARM_STATUS_DISABLE)
#define  MTR_ALARM_PERIOD_SINGLE_EVENT  0

extern uint32_t MTR_systemTime;
extern const uint32_t MTR_systemTimeMax;
extern MTR_AlarmStructType MTR_AlarmStruct;


MTR_Err MTR_Alarm_QueueSort(void);
MTR_Err MTR_Alarm_Evaluate(void);
MTR_Err MTR_Alarm_Configure(MTR_AlarmIndex targetAlarm,MTR_AlarmConfiguration* alarmConfig);
MTR_Err MTR_Alarm_Enable(MTR_AlarmIndex targetAlarmIndex);
MTR_Err MTR_Alarm_Disable(MTR_AlarmIndex targetAlarmIndex);

#endif /* MONITORTIMETRIGGER_H_ */
