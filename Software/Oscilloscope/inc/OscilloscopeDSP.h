#ifndef OSCILLOSCOPEDSP_H_
#define OSCILLOSCOPEDSP_H_

#include "OscilloscopeDisplayManager.h"
#include "OscilloscopeLED.h"
#include "OscilloscopeConfiguration.h"
#include "OscilloscopeAnalog.h"
#include "stdint.h"

#define OSC_DSP_WAVEFORM_HORIZONTAL_POINTS_COUNT                                OSC_DM_MATRIX_COLUMN_COUNT
#define OSC_DSP_WAVEFORM_VERTICAL_POINTS_COUNT                                 (OSC_DM_MATRIX_ROW_COUNT * OSC_DM_MATRIX_ROW_PIXEL_COUNT)
#define OSC_DSP_WAVEFORM_PIXEL_PER_HORIZONTAL_DIVISION                          20
#define OSC_DSP_WAVEFORM_PIXEL_PER_VERTICAL_DIVISION                            10

#define OSC_DSP_DIGITAL_DATA_CORRECTION_COUNT_PER_INVOCATION         1024

#undef  OSC_DSP_CORRECTION
#define OSC_DSP_CORRECTION_SCALE_NUMERATOR                              1
#define OSC_DSP_CORRECTION_SCALE_DENOMINATOR                            1
#define OSC_DSP_CORRECTION_OFFSET                                       0

#define OSC_DSP_RAW_DATA_ZERO_VALUE                                   127
#define OSC_DSP_ZERO_DATA_VERTICAL_PIXEL_COUNT                        (OSC_DSP_WAVEFORM_VERTICAL_POINTS_COUNT / 2)

#define OSC_DSP_DATA_RANGE                                             256
#define OSC_DSP_MAX_DATA_VALUE                                         255
#define OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE                         40960     /*40kByte*/
#define OSC_DSP_INVALID_DATA_VALUE                                     (~0)
#define OSC_DSP_SAMPLE_RATE                                        1000000

extern uint8_t OSC_DSP_Channel_A_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE];
extern uint8_t OSC_DSP_Channel_B_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE];

typedef enum {
  OSC_DSP_Channel_A,
  OSC_DSP_Channel_B
} OSC_DSP_Channel_Type;

typedef enum {
  OSC_DSP_State_Idle,
  OSC_DSP_State_Sampling_PreTriggerMemory_FirstTime,
  OSC_DSP_State_Sampling_WaitingForTrigger,
  OSC_DSP_State_Sampling_PostTriggerMemory,
  OSC_DSP_State_Calculating_UpdatePhase,
  OSC_DSP_State_Calculating_WaveformConstructPhase,
  OSC_DSP_State_Calculating_WaveformDisplayPhase
} OSC_DSP_State_Type;

typedef enum{
  OSC_DSP_CalculationStatus_InProgress,
  OSC_DSP_CalculationStatus_Ready
} OSC_DSP_CalculationStatus_Type;

/*
 * The trigger detection is implemented with Analog watchdog which is able to detect if the signal has
 * left the predefined range, however it can't detect the edge of this transition so the software has to decide.
 * The range always should be set to contain the signal otherwise the interrupts always come so in that case
 * when the signal is on the opposite side of the trigger level as the transition would mean a trigger event
 * then the range should be set to contain the signal and after the trigger level transition it should be set on
 * the other part of the range so after the second transition it must be a trigger event
 */

typedef int32_t OSC_DSP_SampleRate_Type;

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

typedef enum {
  OSC_DSP_DataProcessingMode_Normal   =  OSC_CFG_DATA_PROCESSING_MODE_NORMAL,
  OSC_DSP_DataProcessingMode_Average  =  OSC_CFG_DATA_PROCESSING_MODE_AVERAGE,
  OSC_DSP_DataProcessingMode_Peak     =  OSC_CFG_DATA_PROCESSING_MODE_PEAK
} OSC_DSP_DataProcessingMode_Type;

typedef enum{
  OSC_DSP_DataAcquisitionMode_Single,
  OSC_DSP_DataAcquisitionMode_Repetitive
} OSC_DSP_DataAcquisitionMode_Type;

typedef struct {
  OSC_DSP_State_Type                      dataAcquisitionState;
  int32_t                                 firstDataPosition;
  int32_t                                 preTriggerMemoryLength;
  int32_t                                 postTriggerMemoryLength;
  int32_t                                 triggerPosition;  /*It must be the same for the two channel -> number of samples before trigger*/
  uint8_t                                 triggerLevel;     /*The trigger level in the unprocessed raw data units*/
  OSC_Analog_AnalogWatchdog_Range_Type    triggerAnalogWatchdogRange;
  OSC_DSP_TriggerSlope_Type               triggerSlope;
  OSC_DSP_TriggerSource_Type              triggerSource;
  OSC_DSP_TriggerState_Type               triggerState;
  OSC_DSP_SampleRate_Type                 sampleRate;       /*Number of samples per second -> 1Msample/s -> 1000000*/
} OSC_DSP_StateMachine_Type;

typedef struct {
  int32_t                              virtualTriggerPositionInData;
  int32_t                              triggerPositionOnDisplay;
  int32_t                              samplePerPixel;
  int32_t                              verticalScaleFactorNumerator;
  int32_t                              verticalScaleFactorDenominator;
  int32_t                              verticalOffsetInPixel;
  int32_t                              verticalOffsetIn_mV;
  OSC_DSP_DataProcessingMode_Type      dataProcessingMode;
  uint8_t                              waveformMemoryIndex;
} OSC_DSP_WaveformProperties_Type;

void    OSC_DSP_Init(void);

OSC_DSP_DataAcquisitionMode_Type OSC_DSP_GetDataAcquisitionMode(void);
void    OSC_DSP_Calculate(void);
void    OSC_DSP_ReCalculate(void);
void    OSC_DSP_StartDataAcquisition(void);
void    OSC_DSP_StateMachine_Update(void);
void    OSC_DSP_WaveformProperties_Update(void);
int32_t OSC_DSP_Waveform_VerticalAdjust(int32_t rawData);

uint32_t OSC_DSP_Waveform_CalculateSampleValue(
    OSC_DSP_Channel_Type              channel,
    int32_t                           startIndex,
    int32_t                           dataLength,
    OSC_DSP_DataProcessingMode_Type   dataProcMode
);

OSC_DSP_CalculationStatus_Type OSC_DSP_Waveform_Construct(void);
void OSC_DSP_StateMachine_Update(void);

#endif /* OSCILLOSCOPEDSP_H_ */
