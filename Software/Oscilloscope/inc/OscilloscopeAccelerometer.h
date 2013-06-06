#ifndef OSCILLOSCOPEACCELEROMETER_H_
#define OSCILLOSCOPEACCELEROMETER_H_

#include "stm32f4xx.h"
#include "stm32f4_discovery_lis302dl.h"
#include "MonitorInterface.h"
#include "OscilloscopeView.h"

#define OSC_ACCMETER_CLICK_DATA_CLICK_SRC   0
#define OSC_ACCMETER_CLICK_DATA_X_AXIS      1
#define OSC_ACCMETER_CLICK_DATA_Y_AXIS      2
#define OSC_ACCMETER_CLICK_DATA_Z_AXIS      3

#define OSC_ACCMETER_THRESHOLD_X_AXIS       10
#define OSC_ACCMETER_THRESHOLD_Y_AXIS       10


#define LIS302DL_CLICK_THSx   1            /*1g threshold for x axis*/
#define LIS302DL_CLICK_THSy  (1 << 4)      /*1g threshold for x axis*/
#define LIS302DL_CLICK_THSz   2            /*1g threshold for x axis*/

#define LIS302DL_CLICK_TimeLimit   250   /*125ms - time until click event has to happen*/
#define LIS302DL_CLICK_Latency     200   /*200ms - time until ignores double click after single click*/
#define LIS302DL_CLICK_Window      200   /*200ms - time after latency within the second click has to start*/

#define LIS302DL_CTRL_REG3_ITH_PUSHPULL_CLICK_ITSRC2   0x38
    /*Interrupt is active high and the pin is driven by push-pull output
    the interrupt 1 pin is tied to GND while interrupt 2 pin is driven
    by the click event*/

#define LIS302DL_ITSRC2_PORT          GPIOE
#define LIS302DL_ITSRC2_PIN           GPIO_Pin_1
#define LIS302DL_ITSRC2_EXTI_LINE     EXTI_Line1

#define LIS302DL_ITSRC2_CLK           RCC_AHB1ENR_GPIOEEN

#define LIS302DL_ITSRC2_PRE_PRIO      0x1
#define LIS302DL_ITSRC2_SUB_PRIO      0x7

typedef enum {
  Osc_Accelerometer_State_Idle,
  Osc_Accelerometer_State_ClickEvent,
  Osc_Accelerometer_State_StartAddressTransmit,
  Osc_Accelerometer_State_StartDataReceive,
  Osc_Accelerometer_State_DataRead,
  Osc_Accelerometer_State_DataProcessing
} OSC_AccStateType;

void OSC_Accelerometer_GetClickData(void);
void OSC_Accelerometer_ProcessData(void);
void OSC_Accelerometer_Init(void);

#endif /* OSCILLOSCOPEACCELEROMETER_H_ */
