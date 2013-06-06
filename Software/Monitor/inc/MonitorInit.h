#ifndef MONITORINIT_H_
#define MONITORINIT_H_

#include "MonitorTypes.h"
#include "MonitorInterface.h"
#include "MonitorCfg.h"

extern void (*MTR_InitFuncList[])(void);
extern uint32_t MTR_InitFuncListLength;

void MTR_Init(void);


#endif /* MONITORINIT_H_ */
