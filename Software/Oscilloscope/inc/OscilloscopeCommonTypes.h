#ifndef OSCILLOSCOPECOMMONTYPES_H_
#define OSCILLOSCOPECOMMONTYPES_H_

/* OscilloscopeMenu.h and OscilloscopeSettings.h includes this file in order to share the same events and
 * to avoid cross-include in the same time (rule: higher modules can include lower modules)*/
typedef enum {
  OSC_Settings_Event_Open,
  OSC_Settings_Event_Next,
  OSC_Settings_Event_Previous,
  OSC_Settings_Event_StepUpSingle,
  OSC_Settings_Event_StepUpMultiple,
  OSC_Settings_Event_StepDownSingle,
  OSC_Settings_Event_StepDownMultiple
} OSC_Settings_Event_Type;

#define OSC_VIEW_FULL_SCREEN_UPDATE_NOT_REQUESTED     0
#define OSC_VIEW_FULL_SCREEN_UPDATE_REQUESTED         1

typedef uint8_t OSC_View_FullScreenUpdateRequest_Type;

#endif /* OSCILLOSCOPECOMMONTYPES_H_ */
