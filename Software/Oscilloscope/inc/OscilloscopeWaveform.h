#ifndef OSCILLOSCOPEWAVEFORM_H_
#define OSCILLOSCOPEWAVEFORM_H_

#include "OscilloscopeDisplayManager.h"
#include "OscilloscopeMenu.h"
#include "OscilloscopeSettings.h"
#include "OscilloscopeConfiguration.h"
#include "OscilloscopeDSP.h"
#include "MonitorInterface.h"

typedef enum {
  OSC_Waveform_Event_Open,
  OSC_Waveform_Event_Close,
  OSC_Waveform_Event_VerticalModeSwitch,
  OSC_Waveform_Event_HorizontalModeSwitch,
  OSC_Waveform_Event_HorizontalIncrease,
  OSC_Waveform_Event_HorizontalDecrease,
  OSC_Waveform_Event_VerticalIncrease,
  OSC_Waveform_Event_VerticalDecrease,
  OSC_Waveform_Event_TriggerLevelIncrease,
  OSC_Waveform_Event_TriggerLevelDecrease,
  OSC_Waveform_Event_SingleDataAcquisitionStart,
  OSC_Waveform_Event_Channel_A_Switch,
  OSC_Waveform_Event_Channel_B_Switch
} OSC_Waveform_Event_Type;

typedef enum {
  OSC_Waveform_VerticalMode_Resolution,
  OSC_Waveform_VerticalMode_Offset
} OSC_Waveform_VerticalMode_Type;

typedef enum {
  OSC_Waveform_HorizontalMode_Resolution,
  OSC_Waveform_HorizontalMode_Offset
} OSC_Waveform_HorizontalMode_Type;

void OSC_Waveform_Display(OSC_Waveform_Event_Type waveformEvent);
void OSC_Waveform_DisplayUpdate(void);

#endif /* OSCILLOSCOPEWAVEFORM_H_ */
