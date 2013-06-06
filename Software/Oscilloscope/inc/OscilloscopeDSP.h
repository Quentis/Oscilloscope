#ifndef OSCILLOSCOPEDSP_H_
#define OSCILLOSCOPEDSP_H_

#include "OscilloscopeLCD.h"

#define OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE                         40960     /*20kByte*/
extern uint8_t OSC_DSP_Channel_A_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE];
extern uint8_t OSC_DSP_Channel_B_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE];

typedef enum {
  OSC_DSP_State_Sampling_Single_PreTriggerMemory,
  OSC_DSP_State_Sampling_Circular_PreTriggerMemory,
  OSC_DSP_State_Sampling_Single_PostTriggerMemory_NoOverflow,
  OSC_DSP_State_Sampling_Single_PostTriggerMemory_Overflow,
  OSC_DSP_State_Calculating
} OSC_DSP_State_Type;

typedef enum {
  OSC_DSP_TriggerState_Disabled,
  OSC_DSP_TriggerState_Enabled_TwoMoreInterrupt,
  OSC_DSP_TriggerState_Enabled_NextInterrupt,
  OSC_DSP_TriggerState_Enabled_Active
} OSC_DSP_TriggerState_Type;

typedef enum {
  OSC_DSP_TriggerSource_Channel_A,
  OSC_DSP_TriggerSource_Channel_B
} OSC_DSP_TriggerSource_Type;

typedef struct {
  OSC_DSP_State_Type            dataAcquisitionState;
  OSC_DSP_TriggerState_Type     triggerState;
  OSC_DSP_TriggerSource_Type    triggerSource;

} OSC_DSP_StateMachine;

void OSC_DSP_Init(void);
void OSC_DSP_Calculate(void);

#endif /* OSCILLOSCOPEDSP_H_ */
