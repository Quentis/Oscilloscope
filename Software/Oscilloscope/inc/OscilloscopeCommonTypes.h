#ifndef OSCILLOSCOPECOMMONTYPES_H_
#define OSCILLOSCOPECOMMONTYPES_H_

/* OscilloscopeMenu.h and OscilloscopeSettings.h includes this file in order to share the same events and
 * to avoid cross-include in the same time (rule: higher modules can include lower modules)*/
typedef enum {
  OSC_Menu_Event_Menu_Open,
  OSC_Menu_Event_MenuElement_Next,
  OSC_Menu_Event_MenuElement_Previous,
  OSC_Menu_Event_MenuElement_StepUpSingle,
  OSC_Menu_Event_MenuElement_StepUpMultiple,
  OSC_Menu_Event_MenuElement_StepDownSingle,
  OSC_Menu_Event_MenuElement_StepDownMultiple
} OSC_Menu_Event_Type;

#define OSC_VIEW_FULL_SCREEN_UPDATE_NOT_REQUESTED     0
#define OSC_VIEW_FULL_SCREEN_UPDATE_REQUESTED         1

typedef uint8_t OSC_View_FullScreenUpdateRequest_Type;

#endif /* OSCILLOSCOPECOMMONTYPES_H_ */
