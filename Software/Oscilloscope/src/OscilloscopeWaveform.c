#include "OscilloscopeWaveform.h"

void OSC_Waveform_Display(void){
  //TODO: This only control the OSC_Waveform_DisplayUpdate alarm enable and disable
  //TODO: The display intelligence should be in OSC_Waveform_DisplayUpdate
}

void OSC_Waveform_DisplayUpdate(void){
  static uint32_t divLinesFaint = 0;
  //TODO: Division lines should have lower intensity
  //TODO: Switch between waveform memories when the calculations are ready
  //TODO: Start a new conversion when it is appropriate
  //TODO: Update waveform to the screen
  OSC_DisplayManager_ClearScreen();
  if(divLinesFaint & 0x1) OSC_Waveform_DrawDivLines();

  if(OSC_DSP_StateMachine.dataAcquisitionState == OSC_DSP_State_Idle){
    if(OSC_DSP_StateMachine.dataAcquisitionMode == OSC_DSP_DataAcquisitionMode_Repetitive){
      OSC_DSP_StartDataAcquisition();
    } else {  /*OSC_DSP_StateMachine.dataAcquisitionMode == OSC_DSP_DataAcquisitionMode_Single*/

    }
  } else {    /*OSC_DSP_StateMachine.dataAcquisitionMode != OSC_DSP_State_Idle*/
    if(OSC_DSP_StateMachine.dataAcquisitionMode == OSC_DSP_DataAcquisitionMode_Repetitive){
      uint8_t waveformMemoryIndex = (OSC_DSP_WaveformProperties.waveformMemoryIndex == 0) ? 1 : 0;

      if(OSC_Settings_Channel_A_Status.status == OSC_CFG_CHANNEL_A_STATUS_ENABLED){
        OSC_DisplayManager_Graphics_UpdateWaveform(&OSC_DisplayManager_Waveform_Channel_A[waveformMemoryIndex]);
      }

      if(OSC_Settings_Channel_A_Status.status == OSC_CFG_CHANNEL_A_STATUS_ENABLED){
        OSC_DisplayManager_Graphics_UpdateWaveform(&OSC_DisplayManager_Waveform_Channel_B[waveformMemoryIndex]);
      }
    } else {  /*OSC_DSP_StateMachine.dataAcquisitionMode == OSC_DSP_DataAcquisitionMode_Single*/

    }
  }

}
