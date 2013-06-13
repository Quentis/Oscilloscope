#include "OscilloscopeDSP.h"

/*=================================== DATA ACQUISITION MEMORY DEFINITIONS ===================================*/
uint8_t OSC_DSP_Channel_A_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE];
uint8_t OSC_DSP_Channel_B_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE];

/*===================================== DMA MODE STRUCTURE DEFINITIONS ======================================*/


/*======================================== STATE MACHINE DEFINITIONS ========================================*/
OSC_DSP_StateMachine_Type  OSC_DSP_StateMachine = {
    OSC_DSP_State_Disabled,                         /*dataAcquisitionState*/
    0,                                              /*firstDataPosition*/
    0,                                              /*postTriggerMemoryLength*/
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

  OSC_DSP_StateMachine.postTriggerMemoryLength =
        ((OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE *
        ((OSC_Settings_TriggerPosition.upperBound - OSC_Settings_TriggerPosition.lowerBound) - OSC_Settings_TriggerPosition.value))) /
         (OSC_Settings_TriggerPosition.upperBound - OSC_Settings_TriggerPosition.lowerBound);

  OSC_DSP_StateMachine.triggerLevel = (uint8_t)(
          ( ((OSC_Settings_TriggerLevel.value - OSC_CFG_TRIGGER_LEVEL_LOWER_BOUND)) * OSC_DSP_MAX_DATA_VALUE) /
          (OSC_CFG_TRIGGER_LEVEL_UPPER_BOUND - OSC_CFG_TRIGGER_LEVEL_LOWER_BOUND));
                                        /*Shift is needed for precision*/

  OSC_DSP_StateMachine.triggerSlope =   (OSC_Settings_TriggerSlope.status == OSC_CFG_TRIGGER_SLOPE_RISING) ?
                                         OSC_DSP_TriggerSlope_RisingEdge :
                                         OSC_DSP_TriggerSlope_FallingEdge;

  OSC_DSP_StateMachine.triggerSource =  (OSC_Settings_TriggerSource.status == OSC_CFG_CHANNEL_SELECT_CHANNEL_A_SELECTED) ?
                                         OSC_DSP_TriggerSource_Channel_A :
                                         OSC_DSP_TriggerSource_Channel_B;

  OSC_DSP_StateMachine.triggerState  = OSC_DSP_TriggerState_Disabled;

}

/*
 * The concept is that despite the fact there are two DMA transfer only one interrupt handler will be used.
 * This works because the two ADC are triggered same time and the other DMA transfer (with lower prio) will
 * finish after one DMA operation (1-2 clock cycle) later. Because of the one interrupt handler this must check
 * if the other stream has finished or not (this could be predicted based on the stream priorities but to be on
 * the safe side the code will check this).
 */

void OSC_ANALOG_CHANNEL_A_DMA_STREAM_INTERRUPT_HANDLER(void){
  OSC_Analog_Channel_DataAcquisitionConfig_Type dataAcquisitionConfig_Channel_A, dataAcquisitionConfig_Channel_B;

  while((OSC_ANALOG_CHANNEL_B_DMA_STATUS_REGISTER & OSC_ANALOG_CHANNEL_B_DMA_STATUS_REGISTER_FLAG_TC) !=
         OSC_ANALOG_CHANNEL_B_DMA_STATUS_REGISTER_FLAG_TC);

  switch(OSC_DSP_StateMachine.dataAcquisitionState){
    case OSC_DSP_State_Sampling_Single_PreTriggerMemory:
        dataAcquisitionConfig_Channel_A.dataAcquisitionMemory = OSC_DSP_Channel_A_DataAcquisitionMemory;
        dataAcquisitionConfig_Channel_B.dataAcquisitionMemory = OSC_DSP_Channel_A_DataAcquisitionMemory;
        dataAcquisitionConfig_Channel_A.datalength = OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;
        dataAcquisitionConfig_Channel_B.datalength = OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;
        dataAcquisitionConfig_Channel_A.dmaMode    = OSC_Analog_DMA_Mode_Circular;
        dataAcquisitionConfig_Channel_B.dmaMode    = OSC_Analog_DMA_Mode_Circular;
        OSC_Analog_DMA_ReConfigureBothChannelOnTheFly(0,0);

      break;
    case OSC_DSP_State_Sampling_Single_PostTriggerMemory_NoOverflow:

      break;
    case OSC_DSP_State_Sampling_Single_PostTriggerMemory_Overflow:

      break;
    default: break;
  }
}
