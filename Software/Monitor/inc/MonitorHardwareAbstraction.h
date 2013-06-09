#ifndef MONITORHARDWAREABSTRACTION_H_
#define MONITORHARDWAREABSTRACTION_H_

#include "stm32f4xx_conf.h"
#include "core_cm4.h"

void MTR_HardwareInit(void);
#define MTR_ENABLE_INTERRUPTS()       do{}while(0)
#define MTR_DISABLE_INTERRUPTS()      do{}while(0)

#endif /* MONITORHARDWAREABSTRACTION_H_ */
