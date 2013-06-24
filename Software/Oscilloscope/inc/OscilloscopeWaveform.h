#ifndef OSCILLOSCOPENOTIFICATION_H_
#define OSCILLOSCOPENOTIFICATION_H_

#include "OscilloscopeDisplayManager.h"
#include "OscilloscopeDSP.h"
#include "OscilloscopeMenu.h"

char OSC_Notification_Info[OSC_DM_TEXT_LINE_COUNT][OSC_DM_TEXT_CHARACTER_PER_LINE + 1];  /*+1 because of '\0'*/

void OSC_Notification_PrintAll(void){
  uint32_t index;

  OSC_Menu_StringBuilder(OSC_Menu_Element_TriggerLevel,OSC_Notification_Info[0]);
  OSC_Menu_StringBuilder(OSC_Menu_Element_TriggerPosition,OSC_Notification_Info[1]);
  OSC_Menu_StringBuilder(OSC_Menu_Element_HorizontalResolution,OSC_Notification_Info[2]);
  OSC_Menu_StringBuilder(OSC_Menu_Element_HorizontalOffset,OSC_Notification_Info[3]);
  OSC_Menu_StringBuilder(OSC_Menu_Element_VerticalResolution,OSC_Notification_Info[4]);
  OSC_Menu_StringBuilder(OSC_Menu_Element_VerticalOffset,OSC_Notification_Info[5]);

  for (index = 0; index < OSC_DM_TEXT_LINE_COUNT; ++index) {
    OSC_DisplayManager_Text_PrintLine(OSC_Notification_Info[index],index,OSC_DisplayManager_PrintMode_Positive);
  }
}

#endif /* OSCILLOSCOPENOTIFICATION_H_ */
