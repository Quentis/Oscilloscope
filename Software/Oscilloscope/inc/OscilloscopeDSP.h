#ifndef OSCILLOSCOPEDSP_H_
#define OSCILLOSCOPEDSP_H_

#include "OscilloscopeDisplayManager.h"
#include "OscilloscopeLED.h"
#include "OscilloscopeConfiguration.h"
#include "OscilloscopeAnalog.h"

#define OSC_DSP_WAVEFORM_POINTS_COUNT                                OSC_DM_MATRIX_COLUMN_COUNT

#define OSC_DSP_DIGITAL_DATA_CORRECTION_COUNT_PER_INVOCATION         1024

#define OSC_DSP_CORRECTION_SCALE_NUMERATOR                             1
#define OSC_DSP_CORRECTION_SCALE_DENOMINATOR                           1
#define OSC_DSP_CORRECTION_OFFSET                                      0

#define OSC_DSP_DATA_RANGE                                           256
#define OSC_DSP_MAX_DATA_VALUE                                       255
#define OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE                         40960     /*20kByte*/
extern uint8_t OSC_DSP_Channel_A_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE];
extern uint8_t OSC_DSP_Channel_B_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE];

#define OSC_DSP_SAMPLE_RATE       1000000

typedef enum {
  OSC_DSP_State_Disabled,
  OSC_DSP_State_Sampling_Single_PreTriggerMemory,
  OSC_DSP_State_Sampling_Circular_PreTriggerMemory,
  OSC_DSP_State_Sampling_Single_PostTriggerMemory_NoOverflow,
  OSC_DSP_State_Sampling_Single_PostTriggerMemory_Overflow,
  OSC_DSP_State_Calculating
} OSC_DSP_State_Type;

/*
 * The trigger detection is implemented with Analog watchdog which is able to detect if the signal has
 * left the predefined range, however it can't detect the edge of this transition so the software has to decide.
 * The range always should be set to contain the signal otherwise the interrupts always come so in that case
 * when the signal is on the opposite side of the trigger level as the transition would mean a trigger event
 * then the range should be set to contain the signal and after the trigger level transition it should be set on
 * the other part of the range so after the second transition it must be a trigger event
 */

typedef uint32_t OSC_DSP_SampleRate_Type;

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

typedef enum {
  OSC_DSP_TriggerSlope_RisingEdge,
  OSC_DSP_TriggerSlope_FallingEdge
} OSC_DSP_TriggerSlope_Type;

typedef struct {
  OSC_DSP_State_Type                      dataAcquisitionState;
  uint32_t                                firstDataPosition;
  uint32_t                                preTriggerMemoryLength;
  uint32_t                                postTriggerMemoryLength;
  uint32_t                                triggerPosition;  /*It must be the same for the two channel -> number of samples before trigger*/
  uint8_t                                 triggerLevel;     /*The trigger level in the unprocessed raw data units*/
  OSC_Analog_AnalogWatchdog_Range_Type    triggerAnalogWatchdogRange;
  OSC_DSP_TriggerSlope_Type               triggerSlope;
  OSC_DSP_TriggerSource_Type              triggerSource;
  OSC_DSP_TriggerState_Type               triggerState;
  OSC_DSP_SampleRate_Type                 sampleRate;       /*Number of samples per second -> 1Msample/s -> 1000000*/
} OSC_DSP_StateMachine_Type;

void OSC_DSP_Init(void);
void OSC_DSP_Calculate(void);
void OSC_DSP_StateMachineUpdate(void);

#endif /* OSCILLOSCOPEDSP_H_ */
