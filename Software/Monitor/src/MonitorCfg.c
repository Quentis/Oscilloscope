#include "MonitorCfg.h"

/*====================================INIT_CONFIGURATION===================================*/

/*Initializer functions of the user*/
MTR_InitFuncList_Type MTR_InitFuncList = {
      OSC_Accelerometer_Init,
      OSC_LED_Init,
      OSC_Button_Init,
      OSC_LCD_Init,
      OSC_DisplayManager_Init,
      OSC_Analog_Init
};   /*When the Monitor program starts it calls these functions*/

uint32_t MTR_InitFuncListLength = sizeof(MTR_InitFuncList)/sizeof(MTR_InitFuncList[0]);

/*===================================BACKGROUND_FUNCTIONS==================================*/
/*
 * These functions have very large computational needs or they need to run very often.
 * It is important however to ensure relatively low cycle time for these functions in order
 * to react to alarms and events and let run the other background functions
 */

MTR_BackgroundFunctionsList_Type MTR_BackgroundFunctionsList = {
      OSC_LCD_Transport
};

uint32_t MTR_BackgroundFunctionsListLength = sizeof(MTR_BackgroundFunctionsList)/sizeof(MTR_BackgroundFunctionsList[0]);

/*===================================EVENT_CONFIGURATION===================================*/

static MTR_EventAction MTR_EventList_HighPrio[] = {
      {MTR_EVENT_NAME_CLICK_DATA_IN,  MTR_EVENT_RELAION_OR, OSC_Accelerometer_GetClickData},
      {MTR_EVENT_NAME_CLICK_INTERRUPT,MTR_EVENT_RELAION_OR, OSC_Accelerometer_GetClickData}
};

static MTR_EventAction MTR_EventList_NormalPrio[]  = {
    {0,0,0}     /*DummyEvent*/
};

static MTR_EventAction MTR_EventList_LowPrio[]  = {
    {MTR_EVENT_NAME_DISCOVERY_USER_BTN_PUSH,    MTR_EVENT_RELAION_OR, OSC_ClearLEDs}
};

MTR_EventActionList MTR_eventStruct = {
      MTR_EventList_HighPrio,
      MTR_EventList_NormalPrio,
      MTR_EventList_LowPrio,
      sizeof(MTR_EventList_HighPrio)    / sizeof(MTR_EventList_HighPrio[0]),
      sizeof(MTR_EventList_NormalPrio)  / sizeof(MTR_EventList_NormalPrio[0]),
      sizeof(MTR_EventList_LowPrio)     / sizeof(MTR_EventList_LowPrio[0])
};

/*===================================ALARM_CONFIGURATION===================================*/

static MTR_Alarm MTR_AlarmList[] = {
      {MTR_ALARM_STATUS_DISABLE, MTR_ALARM_EVENTMASK_NOMASK,   10,  0, OSC_TestAlarm_10ms,                ~0},
      {MTR_ALARM_STATUS_DISABLE, MTR_ALARM_EVENTMASK_NOMASK,  100,  0, OSC_TestAlarm_100ms,               ~0},
      {MTR_ALARM_STATUS_DISABLE, MTR_ALARM_EVENTMASK_NOMASK,  500,  0, OSC_TestAlarm_500ms,               ~0},
      {MTR_ALARM_STATUS_DISABLE, MTR_ALARM_EVENTMASK_NOMASK, 1000,  0, OSC_TestAlarm_1000ms,              ~0},
      {MTR_ALARM_STATUS_DISABLE, MTR_ALARM_EVENTMASK_NOMASK,   10,  0, OSC_Button_Read,                   ~0},
      {MTR_ALARM_STATUS_DISABLE, MTR_ALARM_EVENTMASK_NOMASK,   10,  0, OSC_DisplayManager_ScreenUpdater,  ~0},
      {MTR_ALARM_STATUS_DISABLE, MTR_ALARM_EVENTMASK_NOMASK,   10,  0, OSC_Button_StateMachine,           ~0},
      {MTR_ALARM_STATUS_DISABLE, MTR_ALARM_EVENTMASK_NOMASK,   25,  0, OSC_View_EventManager,             ~0}
};
static MTR_AlarmPtr MTR_AlarmPtrList[] = {
      MTR_AlarmList,
      MTR_AlarmList + 1,
      MTR_AlarmList + 2,
      MTR_AlarmList + 3,
      MTR_AlarmList + 4,
      MTR_AlarmList + 5,
      MTR_AlarmList + 6,
      MTR_AlarmList + 7,
};

MTR_AlarmStructType MTR_AlarmStruct = {
    MTR_AlarmPtrList,
    MTR_AlarmList,
    sizeof(MTR_AlarmList)/sizeof(MTR_AlarmList[0])
};
