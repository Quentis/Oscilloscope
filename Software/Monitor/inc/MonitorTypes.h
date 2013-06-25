#ifndef MONITORTYPES_H_
#define MONITORTYPES_H_

#include "stdint.h"

/*======================================ERROR_TYPES======================================*/

typedef   enum{
  MTR_Err_OK,
  MTR_Err_OverFlow,
  MTR_Err_ObjectLocked,
  MTR_Err_ReStart,
  MTR_Err_ReStop,
  MTR_Err_InvalidParam
} MTR_Err;

/*=======================================INIT_TYPES======================================*/

typedef void (*MTR_InitFuncList_Type[])(void);

/*===============================BACKGROUND_FUNCTION_TYPES===============================*/

typedef void (*MTR_BackgroundFunctionsList_Type[])(void);

/*======================================EVENT_TYPES======================================*/

typedef uint32_t                  MTR_EventStatus;
typedef MTR_EventStatus           MTR_EventMask;
typedef uint8_t                   MTR_EventRelation;
typedef uint8_t                   MTR_EventPriority;
typedef void (*MTR_EventCallback)(void);

typedef struct{
  MTR_EventMask         mask;
  MTR_EventRelation     relation;
  MTR_EventCallback     callback;
} MTR_EventAction;

typedef struct{
  MTR_EventAction* MTR_EventList_HighPrio;
  MTR_EventAction* MTR_EventList_NormalPrio;
  MTR_EventAction* MTR_EventList_LowPrio;
  const uint32_t MTR_EventList_HighPrio_Length;
  const uint32_t MTR_EventList_NormalPrio_Length;
  const uint32_t MTR_EventList_LowPrio_Length;
} MTR_EventActionList;

/*======================================ALARM_TYPES======================================*/

typedef uint8_t            MTR_AlarmStatus;
typedef uint32_t           MTR_AlarmIndex;
typedef void (*MTR_AlarmCallback)(void);

typedef struct{
  MTR_AlarmStatus               status;
  uint32_t                      delay;
  uint32_t                      period;      /*period == 0 --> Single event*/
  MTR_EventMask                 eventMask;
  MTR_AlarmCallback             callback;    /*callback == 0 --> No callback invocation*/
  uint32_t                      triggerTime;
} MTR_Alarm;

typedef struct{
  uint32_t                      delay;
  uint32_t                      period;
  MTR_EventMask                 eventMask;
  MTR_AlarmCallback             callback;
} MTR_AlarmConfiguration;

typedef MTR_Alarm*     MTR_AlarmPtr;

typedef struct{
  MTR_AlarmPtr*            ptrList;
  MTR_Alarm*               list;
  const uint32_t           length;
} MTR_AlarmStructType;

/*========================================IO_TYPES=======================================*/

typedef struct{
  void (*sendChar)(uint8_t c);
  MTR_Err (*sendString)(uint8_t str[],uint32_t length);
  uint8_t* outputBuffer;
  uint32_t outputBufferLength;
} MTR_OutDesc;

typedef struct{
  uint8_t (*getChar)();
  MTR_Err (*getString)(uint8_t str[],uint32_t length,uint8_t separator);
  uint8_t* inputBuffer;
  uint32_t inputBufferLength;
  uint8_t separator;
} MTR_InDesc;

#endif /* MONITORTYPES_H_ */
