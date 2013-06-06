#ifndef MONITORINTERFACE_H_
#define MONITORINTERFACE_H_

#include "MonitorTypes.h"
#include "MonitorCfg.h"

/*Initializer functions of the user (extern)*/
extern void OSC_Accelerometer_Init(void);
extern void OSC_LED_Init(void);
extern void OSC_Button_Init(void);
extern void OSC_LCD_Init(void);
extern void OSC_DisplayManager_Init(void);
extern void OSC_Analog_Init(void);

/*========================================EVENT_NAMES========================================*/

#define   MTR_EVENT_NAME_CLICK_DATA_IN                   (1 << 1)
#define   MTR_EVENT_NAME_CLICK_INTERRUPT                 (1 << 2)
#define   MTR_EVENT_NAME_DISCOVERY_USER_BTN_PUSH         (1 << 3)

/*========================================ALARM_NAMES========================================*/
typedef enum {
  MTR_ALARM_NAME_TESTALARM_10ms,
  MTR_ALARM_NAME_TESTALARM_100ms,
  MTR_ALARM_NAME_TESTALARM_500ms,
  MTR_ALARM_NAME_TESTALARM_1000ms,
  MTR_ALARM_NAME_BUTTON_READ,
  MTR_ALARM_NAME_DISPLAY_MANAGER,
  MTR_ALARM_NAME_BUTTON_STATE_MACHINE,
  MTR_ALARM_NAME_VIEW_EVENT_MANAGER
} MTR_Alarm_Name_Type;

/*Background functions of the user*/
extern void OSC_LCD_Transport(void);

/*Event callbacks of the user (extern)*/
extern void OSC_ClearLEDs(void);
extern void OSC_Accelerometer_GetClickData(void);

/*Alarm callbacks of the user (extern)*/
extern void OSC_TestAlarm_10ms(void);
extern void OSC_TestAlarm_100ms(void);
extern void OSC_TestAlarm_500ms(void);
extern void OSC_TestAlarm_1000ms(void);
extern void OSC_Button_Read(void);
extern void OSC_DisplayManager_ScreenUpdater(void);
extern void OSC_Button_StateMachine(void);
extern void OSC_View_EventManager(void);

/*Monitor Initializer function*/
extern void MTR_Init(void);

/*Monitor API functions*/
extern  MTR_Err         MTR_Event_Set(MTR_EventMask mask);
extern  MTR_EventMask   MTR_Event_GetStatus(void);
extern  MTR_Err         MTR_Event_Evaluate(MTR_EventPriority prio);
extern  MTR_Err         MTR_Event_Clear(MTR_EventMask mask);

extern  MTR_Err         MTR_Alarm_Configure(MTR_AlarmIndex targetAlarm,MTR_AlarmConfiguration* alarmConfig);
extern  MTR_Err         MTR_Alarm_Enable(MTR_AlarmIndex targetAlarmIndex);
extern  MTR_Err         MTR_Alarm_Disable(MTR_AlarmIndex targetAlarmIndex);

#endif /* MONITORINTERFACE_H_ */
