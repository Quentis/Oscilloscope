#ifndef MONITOREVENTS_H_
#define MONITOREVENTS_H_

#include "stdint.h"
#include "MonitorTypes.h"
#include "MonitorUtils.h"

#define   MTR_EVENT_RELAION_OR     0
#define   MTR_EVENT_RELAION_AND    1

#define  MTR_EVENT_HIGHPRIO       (1 << 2)
#define  MTR_EVENT_NORMALPRIO     (1 << 1)
#define  MTR_EVENT_LOWPRIO        (1 << 0)

extern MTR_EventActionList MTR_eventStruct;

MTR_Err         MTR_Event_Set(MTR_EventMask mask);
MTR_EventMask   MTR_Event_GetStatus(void);
MTR_Err         MTR_Event_Evaluate(MTR_EventPriority prio);
MTR_Err         MTR_Event_Clear(MTR_EventMask mask);

#endif /* MONITOREVENTS_H_ */
