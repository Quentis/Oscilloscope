#ifndef MONITORCFG_H_
#define MONITORCFG_H_

#include "MonitorEvent.h"
#include "MonitorScheduler.h"
#include "MonitorTypes.h"
#include "MonitorInterface.h"

#define   MTR_SCHEDULER_FREQ    1000    /*in Hertz*/

#define   MTR_EVENT_HIGHPRIO_POLLFREQ         1      /*In systemTime ticks*/
#define   MTR_EVENT_NORMALPRIO_POLLFREQ      10      /*In systemTime ticks*/
#define   MTR_EVENT_LOWPRIO_POLLFREQ        100      /*In systemTime ticks*/

#define   MTR_EVENT_HIGHPRIO_DELAY            0      /*In systemTime ticks*/
#define   MTR_EVENT_NORMALPRIO_DELAY          5      /*In systemTime ticks*/
#define   MTR_EVENT_LOWPRIO_DELAY             0      /*In systemTime ticks*/

/*Extern symbols of the user defined callbacks*/

#endif /* MONITORCFG_H_ */
