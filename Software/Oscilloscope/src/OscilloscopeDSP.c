#include "OscilloscopeDSP.h"

uint8_t OSC_DSP_Channel_A_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE];
uint8_t OSC_DSP_Channel_B_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE];

OSC_DSP_StateMachine_Type  OSC_DSP_StateMachine = {
    OSC_DSP_State_Disabled,                         /*dataAcquisitionState*/
    OSC_DSP_State_Disabled,                         /*dataAcquisitionState_Channel_A*/
    OSC_DSP_State_Disabled,                         /*dataAcquisitionState_Channel_B*/
    0,                                              /*triggerPosition*/
    127,                                            /*triggerLevel*/
    OSC_DSP_TriggerSlope_RisingEdge,                /*triggerSlope*/
    OSC_DSP_TriggerSource_Channel_A,                /*triggerSource*/
    OSC_DSP_TriggerState_Disabled,                  /*triggerState*/
    OSC_DSP_SAMPLE_RATE                             /*sampleRate*/
};

void OSC_DSP_Init(void){

}

void OSC_DSP_Calculate(void){

}

void OSC_DSP_StateMachineUpdate(void){
  OSC_DSP_StateMachine.dataAcquisitionState           = OSC_DSP_State_Disabled;
  OSC_DSP_StateMachine.dataAcquisitionState_Channel_A = OSC_DSP_State_Disabled;
  OSC_DSP_StateMachine.dataAcquisitionState_Channel_B = OSC_DSP_State_Disabled;

  OSC_DSP_StateMachine.triggerPosition;

  OSC_DSP_StateMachine.triggerLevel = (uint8_t)(
          (( ((OSC_Settings_TriggerLevel.value - OSC_CFG_TRIGGER_LEVEL_LOWER_BOUND) << 16) * OSC_DSP_MAX_DATA_VALUE) /
          (OSC_CFG_TRIGGER_LEVEL_UPPER_BOUND - OSC_CFG_TRIGGER_LEVEL_LOWER_BOUND)) >> 16);
                                        /*Shift is needed for precision*/

  OSC_DSP_StateMachine.triggerSlope =   (OSC_Settings_TriggerSlope.status == OSC_CFG_TRIGGER_SLOPE_RISING) ?
                                         OSC_DSP_TriggerSlope_RisingEdge :
                                         OSC_DSP_TriggerSlope_FallingEdge;

  OSC_DSP_StateMachine.triggerSource =  (OSC_Settings_TriggerSource.status == OSC_CFG_CHANNEL_SELECT_CHANNEL_A_SELECTED) ?
                                         OSC_DSP_TriggerSource_Channel_A :
                                         OSC_DSP_TriggerSource_Channel_B;

  OSC_DSP_StateMachine.triggerState  = OSC_DSP_TriggerState_Disabled;


}
