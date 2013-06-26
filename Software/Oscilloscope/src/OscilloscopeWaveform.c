#include "OscilloscopeWaveform.h"

void OSC_Waveform_Display(OSC_Waveform_Event_Type waveformEvent){
  static OSC_Waveform_VerticalMode_Type   OSC_Waveform_VerticalMode   = OSC_Waveform_VerticalMode_Resolution;
  static OSC_Waveform_HorizontalMode_Type OSC_Waveform_HorizontalMode = OSC_Waveform_HorizontalMode_Resolution;

  switch(waveformEvent){
    case OSC_Waveform_Event_Open:
      MTR_Alarm_Enable(MTR_ALARM_NAME_WAVEFORM_DISPLAY_UPDATE);
      break;
    case OSC_Waveform_Event_Close:
      MTR_Alarm_Disable(MTR_ALARM_NAME_WAVEFORM_DISPLAY_UPDATE);
      break;
    case OSC_Waveform_Event_VerticalModeSwitch:
      if(OSC_Waveform_VerticalMode == OSC_Waveform_VerticalMode_Resolution){
        OSC_Waveform_VerticalMode = OSC_Waveform_VerticalMode_Offset;
      } else {    /*OSC_Waveform_VerticalMode == OSC_Waveform_VerticalMode_Offset*/
        OSC_Waveform_VerticalMode = OSC_Waveform_VerticalMode_Resolution;
      }
      break;
    case OSC_Waveform_Event_HorizontalModeSwitch:
      if(OSC_Waveform_HorizontalMode == OSC_Waveform_HorizontalMode_Resolution){
        OSC_Waveform_HorizontalMode = OSC_Waveform_HorizontalMode_Offset;
      } else {    /*OSC_Waveform_HorizontalMode == OSC_Waveform_HorizontalMode_Offset*/
        OSC_Waveform_HorizontalMode = OSC_Waveform_HorizontalMode_Resolution;
      }
      break;
    case OSC_Waveform_Event_HorizontalIncrease:
      if(OSC_Waveform_HorizontalMode == OSC_Waveform_HorizontalMode_Resolution){
        OSC_Settings_Modify(&OSC_Settings_HorizontalResolution,OSC_Settings_Event_StepUpSingle);
      } else {    /*OSC_Waveform_HorizontalMode == OSC_Waveform_HorizontalMode_Offset*/
        OSC_Settings_Modify(&OSC_Settings_TriggerPosition,OSC_Settings_Event_StepUpSingle);
      }
      break;
    case OSC_Waveform_Event_HorizontalDecrease:
      if(OSC_Waveform_HorizontalMode == OSC_Waveform_HorizontalMode_Resolution){
        OSC_Settings_Modify(&OSC_Settings_HorizontalResolution,OSC_Settings_Event_StepDownSingle);
      } else {    /*OSC_Waveform_HorizontalMode == OSC_Waveform_HorizontalMode_Offset*/
        OSC_Settings_Modify(&OSC_Settings_TriggerPosition,OSC_Settings_Event_StepDownSingle);
      }
      break;
    case OSC_Waveform_Event_VerticalIncrease:
      if(OSC_Waveform_VerticalMode == OSC_Waveform_VerticalMode_Resolution){
        OSC_Settings_Modify(&OSC_Settings_VerticalResolution,OSC_Settings_Event_StepUpSingle);
      } else {    /*OSC_Waveform_VerticalMode == OSC_Waveform_VerticalMode_Offset*/
        OSC_Settings_Modify(&OSC_Settings_VerticalOffset,OSC_Settings_Event_StepUpSingle);
      }
      break;
    case OSC_Waveform_Event_VerticalDecrease:
      if(OSC_Waveform_VerticalMode == OSC_Waveform_VerticalMode_Resolution){
        OSC_Settings_Modify(&OSC_Settings_VerticalResolution,OSC_Settings_Event_StepDownSingle);
      } else {    /*OSC_Waveform_VerticalMode == OSC_Waveform_VerticalMode_Offset*/
        OSC_Settings_Modify(&OSC_Settings_VerticalOffset,OSC_Settings_Event_StepDownSingle);
      }
      break;
    case OSC_Waveform_Event_TriggerLevelIncrease:
      OSC_Settings_Modify(&OSC_Settings_TriggerLevel,OSC_Settings_Event_StepUpMultiple);
      break;
    case OSC_Waveform_Event_TriggerLevelDecrease:
      OSC_Settings_Modify(&OSC_Settings_TriggerLevel,OSC_Settings_Event_StepDownMultiple);
      break;
    case OSC_Waveform_Event_SingleDataAcquisitionStart:
      OSC_DSP_StartDataAcquisition();
      break;
    case OSC_Waveform_Event_Channel_A_Switch:
      if(OSC_Settings_Channel_A_Status_Object.status == OSC_CFG_CHANNEL_A_STATUS_ENABLED){
        OSC_Settings_Modify(&OSC_Settings_Channel_A_Status,OSC_Settings_Event_StepUpSingle);
      } else {    /*OSC_Settings_Channel_A_Status.status == OSC_CFG_CHANNEL_A_STATUS_DISABLED*/
        OSC_Settings_Modify(&OSC_Settings_Channel_A_Status,OSC_Settings_Event_StepDownSingle);
      }
      break;
    case OSC_Waveform_Event_Channel_B_Switch:
      if(OSC_Settings_Channel_B_Status_Object.status == OSC_CFG_CHANNEL_B_STATUS_ENABLED){
        OSC_Settings_Modify(&OSC_Settings_Channel_B_Status,OSC_Settings_Event_StepUpSingle);
      } else {    /*OSC_Settings_Channel_B_Status.status == OSC_CFG_CHANNEL_B_STATUS_DISABLED*/
        OSC_Settings_Modify(&OSC_Settings_Channel_B_Status,OSC_Settings_Event_StepDownSingle);
      }
      break;
  }
}

void OSC_Waveform_DisplayUpdate(void){
  uint8_t waveformMemoryIndex;

  OSC_DisplayManager_ClearScreen();
  OSC_DisplayManager_Graphics_DrawDivLines(); /*The division lines will have lower intensity*/

  waveformMemoryIndex = OSC_DSP_Waveform_GetLastUpdatedMemoryIndex();

  if(OSC_DSP_GetDataProcessingStatus() == OSC_DSP_DataProcessingStatus_Ready){
    if(OSC_DSP_GetDataAcquisitionMode() == OSC_DSP_DataAcquisitionMode_Repetitive){
      OSC_DSP_StartDataAcquisition();
    } else {
      OSC_DSP_ReCalculate();
    }
  }

  if(OSC_Settings_Channel_A_Status_Object.status == OSC_CFG_CHANNEL_A_STATUS_ENABLED){
    OSC_DisplayManager_Graphics_UpdateWaveform(&OSC_DisplayManager_Waveform_Channel_A[waveformMemoryIndex]);
  }

  if(OSC_Settings_Channel_B_Status_Object.status == OSC_CFG_CHANNEL_B_STATUS_ENABLED){
    OSC_DisplayManager_Graphics_UpdateWaveform(&OSC_DisplayManager_Waveform_Channel_B[waveformMemoryIndex]);
  }
}
