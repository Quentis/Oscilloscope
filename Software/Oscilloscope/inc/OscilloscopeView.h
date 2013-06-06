#ifndef OSCILLOSCOPEVIEW_H_
#define OSCILLOSCOPEVIEW_H_

#include "stdint.h"
#include "OscilloscopeMenu.h"

#define   OSC_VIEW_EVENTFIFO_SIZE         64

typedef enum {
  OSC_View_Event_EmptyEvent,
  OSC_View_Event_HorizontalLeftClickSingle,
  OSC_View_Event_HorizontalLeftClickDouble,
  OSC_View_Event_HorizontalMiddleClickSingle,
  OSC_View_Event_HorizontalMiddleClickDouble,
  OSC_View_Event_HorizontalRightClickSingle,
  OSC_View_Event_HorizontalRightClickDouble,
  OSC_View_Event_VerticalUpClickSingle,
  OSC_View_Event_VerticalUpClickDouble,
  OSC_View_Event_VerticalMiddleClickSingle,
  OSC_View_Event_VerticalMiddleClickDouble,
  OSC_View_Event_VerticalDownClickSingle,
  OSC_View_Event_VerticalDownClickDouble,
  OSC_View_Event_TriggerUpClickSingle,
  OSC_View_Event_TriggerUpClickDouble,
  OSC_View_Event_TriggerDownClickSingle,
  OSC_View_Event_TriggerDownClickDouble,
  OSC_View_Event_TurnLeft,
  OSC_View_Event_TurnUp,
  OSC_View_Event_TurnRight,
  OSC_View_Event_TurnDown
} OSC_View_Event_Type;

typedef uint32_t OSC_View_EventFifoIndex_Type;

OSC_View_EventFifoIndex_Type  OSC_View_EventFifoIndex_FirstFree;
OSC_View_EventFifoIndex_Type  OSC_View_EventFifoIndex_FirstIn;
OSC_View_Event_Type OSC_View_EventFifo[OSC_VIEW_EVENTFIFO_SIZE];

typedef enum {
  OSC_View_Err_OK,
  OSC_View_Err_FifoFull
} OSC_View_Err_Type;

typedef enum {
  OSC_View_DisplayController_Waveform,
  OSC_View_DisplayController_Notification,
  OSC_View_DisplayController_Menu
} OSC_View_DisplayController_Type;

typedef struct {
  OSC_View_DisplayController_Type   displayController;
  OSC_Menu_Name_Type                activeMenuName;
} OSC_View_State_Type;

OSC_View_Err_Type     OSC_View_Event_RegisterEvent(OSC_View_Event_Type event);
OSC_View_Event_Type   OSC_View_Event_GetEvent(void);
void                  OSC_View_EventManager(void);

#endif /* OSCILLOSCOPEVIEW_H_ */
