#include "OscilloscopeDSP.h"

/*=================================== DATA ACQUISITION MEMORY DEFINITIONS ===================================*/
uint8_t OSC_DSP_Channel_A_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE];
uint8_t OSC_DSP_Channel_B_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE];

/*===================================== DMA MODE STRUCTURE DEFINITIONS ======================================*/
static const OSC_Analog_Channel_DataAcquisitionConfig_Type OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PreTrigger = {
    OSC_DSP_Channel_A_DataAcquisitionMemory,
    OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE,
    OSC_Analog_DMA_Mode_Normal
};

static const OSC_Analog_Channel_DataAcquisitionConfig_Type OSC_Analog_Channel_B_DataAcquisitionConfig_Normal_PreTrigger = {
    OSC_DSP_Channel_B_DataAcquisitionMemory,
    OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE,
    OSC_Analog_DMA_Mode_Normal
};

static const OSC_Analog_Channel_DataAcquisitionConfig_Type OSC_Analog_Channel_A_DataAcquisitionConfig_Circular_PreTrigger = {
    OSC_DSP_Channel_A_DataAcquisitionMemory,
    OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE,
    OSC_Analog_DMA_Mode_Circular
};

static const OSC_Analog_Channel_DataAcquisitionConfig_Type OSC_Analog_Channel_B_DataAcquisitionConfig_Circular_PreTrigger = {
    OSC_DSP_Channel_B_DataAcquisitionMemory,
    OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE,
    OSC_Analog_DMA_Mode_Circular
};

/*In case of postTrigger sampling the length and the start address changes from data acquisition to data acquisition*/
static OSC_Analog_Channel_DataAcquisitionConfig_Type OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger = {
    0,
    0,
    OSC_Analog_DMA_Mode_Normal
};

static OSC_Analog_Channel_DataAcquisitionConfig_Type OSC_Analog_Channel_B_DataAcquisitionConfig_Normal_PostTrigger = {
    0,
    0,
    OSC_Analog_DMA_Mode_Normal
};

/*======================================== STATE MACHINE DEFINITIONS ========================================*/
OSC_DSP_StateMachine_Type  OSC_DSP_StateMachine = {
    OSC_DSP_State_Disabled,                         /*dataAcquisitionState*/
    0,                                              /*firstDataPosition*/
    0,                                              /*preTriggerMemoryLength*/
    0,                                              /*postTriggerMemoryLength*/
    0,                                              /*triggerPosition*/
    127,                                            /*triggerLevel*/
    OSC_Analog_AnalogWatchdog_Range_Invalid,        /*triggerAnalogWatchdogRange*/
    OSC_DSP_TriggerSlope_RisingEdge,                /*triggerSlope*/
    OSC_DSP_TriggerSource_Channel_A,                /*triggerSource*/
    OSC_DSP_TriggerState_Disabled,                  /*triggerState*/
    OSC_DSP_SAMPLE_RATE                             /*sampleRate*/
};

void OSC_DSP_Init(void){

}

void OSC_DSP_Calculate(void){

}

void OSC_DSP_StateMachineUpdate(void){    /*Updates the DSP state machine configuration dependent attributes*/
  OSC_DSP_StateMachine.dataAcquisitionState           = OSC_DSP_State_Disabled;

  OSC_DSP_StateMachine.preTriggerMemoryLength =
        (OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE * OSC_Settings_TriggerPosition.value) /
         (OSC_Settings_TriggerPosition.upperBound - OSC_Settings_TriggerPosition.lowerBound);

  OSC_DSP_StateMachine.postTriggerMemoryLength = OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE - OSC_DSP_StateMachine.preTriggerMemoryLength;

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
  uint8_t actualValue;

  while((OSC_ANALOG_CHANNEL_B_DMA_STATUS_REGISTER & OSC_ANALOG_CHANNEL_B_DMA_STATUS_REGISTER_FLAG_TC) !=
         OSC_ANALOG_CHANNEL_B_DMA_STATUS_REGISTER_FLAG_TC);

  switch(OSC_DSP_StateMachine.dataAcquisitionState){
    case OSC_DSP_State_Sampling_Single_PreTriggerMemory:
        OSC_Analog_DMA_ReConfigureBothChannelOnTheFly(
            &OSC_Analog_Channel_A_DataAcquisitionConfig_Circular_PreTrigger,
            &OSC_Analog_Channel_B_DataAcquisitionConfig_Circular_PreTrigger
        );
        OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Sampling_Circular_PreTriggerMemory;

        actualValue = OSC_DSP_Channel_A_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE - 1];
        /*the last converted data is in the end of the array*/

        if(actualValue > OSC_DSP_StateMachine.triggerLevel){
          OSC_DSP_StateMachine.triggerAnalogWatchdogRange = OSC_Analog_AnalogWatchdog_Range_Upper;
          if(OSC_DSP_StateMachine.triggerSlope == OSC_DSP_TriggerSlope_RisingEdge){
            OSC_DSP_StateMachine.triggerState = OSC_DSP_TriggerState_Enabled_TwoMoreInterrupt;
          } else {  /*OSC_DSP_StateMachine.triggerSlope == OSC_DSP_TriggerSlope_FallingEdge*/
            OSC_DSP_StateMachine.triggerState = OSC_DSP_TriggerState_Enabled_NextInterrupt;
          }

        } else {    /*actualValue <= OSC_DSP_StateMachine.triggerLevel*/
          OSC_DSP_StateMachine.triggerAnalogWatchdogRange = OSC_Analog_AnalogWatchdog_Range_Lower;
          if(OSC_DSP_StateMachine.triggerSlope == OSC_DSP_TriggerSlope_RisingEdge){
            OSC_DSP_StateMachine.triggerState = OSC_DSP_TriggerState_Enabled_NextInterrupt;
          } else {  /*OSC_DSP_StateMachine.triggerSlope == OSC_DSP_TriggerSlope_FallingEdge*/
            OSC_DSP_StateMachine.triggerState = OSC_DSP_TriggerState_Enabled_TwoMoreInterrupt;
          }
        }

        switch(OSC_DSP_StateMachine.triggerSource){
          case OSC_DSP_TriggerSource_Channel_A:
            OSC_Analog_AnalogWatchdog_Enable(OSC_ANALOG_CHANNEL_A_ADC,OSC_DSP_StateMachine.triggerLevel,OSC_DSP_StateMachine.triggerAnalogWatchdogRange);
            break;
          case OSC_DSP_TriggerSource_Channel_B:
            OSC_Analog_AnalogWatchdog_Enable(OSC_ANALOG_CHANNEL_B_ADC,OSC_DSP_StateMachine.triggerLevel,OSC_DSP_StateMachine.triggerAnalogWatchdogRange);
            break;
          default:
            break;
        }
      break;
    case OSC_DSP_State_Sampling_Single_PostTriggerMemory_NoOverflow:

      break;
    case OSC_DSP_State_Sampling_Single_PostTriggerMemory_Overflow:

      break;
    default: break;
  }
}

void OSC_ANALOG_ADC_INTERRUPT_HANDLER(void){  /*There is one interrupt for all of the ADCs*/
  DMA_Stream_TypeDef* triggerSourceChannelDMA;
  uint32_t initializedDataLength;
  uint32_t dataLengthRegisterDMA;
  uint32_t fistPostTriggerDataPosition;

  switch(OSC_DSP_StateMachine.triggerState){
    ADC_TypeDef* triggerSourceADC;

    case OSC_DSP_TriggerState_Enabled_TwoMoreInterrupt:
      OSC_DSP_StateMachine.triggerState = OSC_DSP_TriggerState_Enabled_NextInterrupt;
      OSC_Analog_AnalogWatchdog_Disable();

      if(OSC_DSP_StateMachine.triggerSource == OSC_DSP_TriggerSource_Channel_A){
        triggerSourceADC = OSC_ANALOG_CHANNEL_A_ADC;
      } else {  /*OSC_DSP_StateMachine.triggerSource == OSC_DSP_TriggerSource_Channel_B*/
        triggerSourceADC = OSC_ANALOG_CHANNEL_B_ADC;
      }

      if(OSC_DSP_StateMachine.triggerAnalogWatchdogRange == OSC_Analog_AnalogWatchdog_Range_Upper){
        OSC_DSP_StateMachine.triggerAnalogWatchdogRange = OSC_Analog_AnalogWatchdog_Range_Lower;
      } else {    /*OSC_DSP_StateMachine.triggerAnalogWatchdogRange == OSC_Analog_AnalogWatchdog_Range_Lower*/
        OSC_DSP_StateMachine.triggerAnalogWatchdogRange = OSC_Analog_AnalogWatchdog_Range_Upper;
      }
      OSC_Analog_AnalogWatchdog_Enable(triggerSourceADC,OSC_DSP_StateMachine.triggerLevel,OSC_DSP_StateMachine.triggerAnalogWatchdogRange);
      break;
    case OSC_DSP_TriggerState_Enabled_NextInterrupt:
      OSC_DSP_StateMachine.triggerState = OSC_DSP_TriggerState_Enabled_Active;
      break;
    default:
      OSC_DSP_StateMachine.triggerState = OSC_DSP_TriggerState_Disabled;
      OSC_LED_TRIGGER_SET();
      break;
  }

  if(OSC_DSP_StateMachine.triggerState == OSC_DSP_TriggerState_Enabled_Active){
    if(OSC_DSP_StateMachine.dataAcquisitionState == OSC_DSP_State_Sampling_Circular_PreTriggerMemory){

      /*
       * Fifo flush can be started here -> it will be ready when DMA reconfiguration will be called
       *
       * It is important because of the data consistency -> The read data could be out of date because of hardware events
       *
       * DMA transfers won't be lost because of the disable because if new data is available in the ADC register then
       * DMA request will be active as long as the DMA does not answer with an acknowledge which will be sent after
       * the reactivation of the DMA
       *
       * This interrupt is generated immediately after a conversion (new value -> analog watchdog check)
       *
       * This interrupt should be finished in 2 ADC conversion time because the trigger position value is passed to the
       * DMA before the deactivation so when this interrupt starts the data register is empty. After the next conversion
       * is ready a DMA request is sent and this must be serviced before the next conversion in order to avoid overflow
       * Time constraint: 2 * 168 = 336 clock cycle (168MHz and 1Msample/s)
       */
      OSC_ANALOG_CHANNEL_A_DMA_STREAM_STOP();
      OSC_ANALOG_CHANNEL_B_DMA_STREAM_STOP();

      if(OSC_DSP_StateMachine.postTriggerMemoryLength > 1){
        switch(OSC_DSP_StateMachine.triggerSource){
          case OSC_DSP_TriggerSource_Channel_A:
            triggerSourceChannelDMA = OSC_ANALOG_CHANNEL_A_DMA_STREAM;
            dataLengthRegisterDMA   = OSC_ANALOG_CHANNEL_A_DMA_STREAM_DATA_LENGTH_GET();
            initializedDataLength   = OSC_Analog_Channel_A_DataAcquisitionConfig_Circular_PreTrigger.datalength;
            break;
          case OSC_DSP_TriggerSource_Channel_B:
            triggerSourceChannelDMA = OSC_ANALOG_CHANNEL_B_DMA_STREAM;
            dataLengthRegisterDMA   = OSC_ANALOG_CHANNEL_B_DMA_STREAM_DATA_LENGTH_GET();
            initializedDataLength = OSC_Analog_Channel_B_DataAcquisitionConfig_Circular_PreTrigger.datalength;
            break;
          default:
            break;
        }
        OSC_DSP_StateMachine.triggerPosition   = initializedDataLength - dataLengthRegisterDMA;   /*It can't be negative*/
        OSC_DSP_StateMachine.firstDataPosition = OSC_DSP_StateMachine.triggerPosition + OSC_DSP_StateMachine.postTriggerMemoryLength;
        fistPostTriggerDataPosition = ((OSC_DSP_StateMachine.triggerPosition + 1) > OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE) ?
                                       (OSC_DSP_StateMachine.triggerPosition + 1) : 0;

        OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger.dataAcquisitionMemory =
            &OSC_DSP_Channel_A_DataAcquisitionMemory[fistPostTriggerDataPosition];
        OSC_Analog_Channel_B_DataAcquisitionConfig_Normal_PostTrigger.dataAcquisitionMemory =
            &OSC_DSP_Channel_B_DataAcquisitionMemory[fistPostTriggerDataPosition];

        if(OSC_DSP_StateMachine.firstDataPosition < OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE){
          OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Sampling_Single_PostTriggerMemory_NoOverflow;
          OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger.datalength =
              OSC_DSP_StateMachine.firstDataPosition - fistPostTriggerDataPosition;   /*triggerPosition data has already been transmitted*/
        } else {  /*OSC_DSP_StateMachine.firstDataPosition >= OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE*/
          OSC_DSP_StateMachine.firstDataPosition   -= OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;
          OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger.datalength =
              OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE - fistPostTriggerDataPosition;     /*triggerPosition data has already been transmitted*/
          OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Sampling_Single_PostTriggerMemory_Overflow;
        }

        OSC_Analog_DMA_ReConfigureBothChannelOnTheFly(      /*dmaMode in the structure is set at the beginning of the program and it doesn't change*/
                    &OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger,
                    &OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger
                );
        /*At this point it must be in 336 clock cycle since the start of the interrupt*/

        } else {  /*OSC_DSP_StateMachine.postTriggerMemoryLength <= 1*/
          /*The condition contains 1 instead of 0 because the triggerPosition data is the part of the postTrigger data*/
          OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Calculating;
          /*TODO: Other administrative and disables*/
        }
    } else {
      /*TODO: EPICFAIL*/
    }
  } /*OSC_DSP_StateMachine.triggerState == OSC_DSP_TriggerState_Enabled_Active*/
}
