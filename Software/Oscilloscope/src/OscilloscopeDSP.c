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
    OSC_DSP_State_Disabled,                         /*dataAcquisitionState*/              /*COMPUTED*/
    0,                                              /*firstDataPosition*/                 /*COMPUTED*/
    0,                                              /*preTriggerMemoryLength*/            /*UPDATE-FROM-CONFIG*/
    0,                                              /*postTriggerMemoryLength*/           /*UPDATE-FROM-CONFIG*/
    0,                                              /*triggerPosition*/                   /*COMPUTED*/
    127,                                            /*triggerLevel*/                      /*UPDATE-FROM-CONFIG*/
    OSC_Analog_AnalogWatchdog_Range_Invalid,        /*triggerAnalogWatchdogRange*/        /*COMPUTED*/
    OSC_DSP_TriggerSlope_RisingEdge,                /*triggerSlope*/                      /*UPDATE-FROM-CONFIG*/
    OSC_DSP_TriggerSource_Channel_A,                /*triggerSource*/                     /*UPDATE-FROM-CONFIG*/
    OSC_DSP_TriggerState_Disabled,                  /*triggerState*/                      /*COMPUTED*/
    OSC_DSP_SAMPLE_RATE                             /*sampleRate*/                        /*UPDATE-FROM-CONFIG*/
    /*FIXME: It is not used in version 1.0 --> The timer should be adjusted according to this*/
};

/*===================================== WAVEFORM PROPERTIES DEFINITIONS =====================================*/
OSC_DSP_WaveformProperties_Type OSC_DSP_WaveformProperties = {
    0,                                                 /*virtualTriggerPosition*/
    0,                                                 /*triggerPositionOnDisplay*/
    0,                                                 /*samplePerPixel*/
    0,                                                 /*verticalScaleFactorNumerator*/
    0,                                                 /*verticalScaleFactorDenominator*/
    0,                                                 /*verticalOffsetInPixel*/
    0,                                                 /*verticalOffsetIn_mV*/
    OSC_DSP_DataProcessingMode_Normal                  /*dataProcessingMode*/
};

/*====================================== EXTERNAL FUNCTION DEFINITIONS ======================================*/

void OSC_DSP_Init(void){

}

void OSC_DSP_StartDataAcquisition(void){
  OSC_DSP_StateMachine_Update();
  OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Sampling_Single_PreTriggerMemory;
  OSC_DSP_StateMachine.triggerState = OSC_DSP_TriggerState_Disabled;
  OSC_DSP_StateMachine.triggerAnalogWatchdogRange = OSC_Analog_AnalogWatchdog_Range_Invalid;

  OSC_Analog_DMA_ReConfigureBothChannelOnTheFly(
      &OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PreTrigger,
      &OSC_Analog_Channel_B_DataAcquisitionConfig_Normal_PreTrigger
  );

  OSC_Analog_Conversion_Start(OSC_Analog_ChannelSelect_ChannelBoth);
  DMA_ITConfig(OSC_ANALOG_CHANNEL_A_DMA_STREAM,OSC_ANALOG_CHANNEL_A_DMA_STATUS_REGISTER_ITFLAG_TC,ENABLE);
}

void OSC_DSP_Calculate(void){
  OSC_DSP_CalculationStatus_Type calcultaionStatus;
  #ifdef OSC_DSP_CORRECTION
  static uint32_t index = 0;
  uint32_t endIndex;
  #endif

  #ifdef OSC_DSP_CORRECTION
  endIndex = index + OSC_DSP_DIGITAL_DATA_CORRECTION_COUNT_PER_INVOCATION;
  if(endIndex > OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE){
    endIndex = OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;
  }

  for(;index < endIndex; index++){
    OSC_DSP_Channel_A_DataAcquisitionMemory[index] =
        ((OSC_DSP_Channel_A_DataAcquisitionMemory[index] * OSC_DSP_CORRECTION_SCALE_NUMERATOR) /
          OSC_DSP_CORRECTION_SCALE_DENOMINATOR) + OSC_DSP_CORRECTION_OFFSET;
    OSC_DSP_Channel_B_DataAcquisitionMemory[index] =
        ((OSC_DSP_Channel_B_DataAcquisitionMemory[index] * OSC_DSP_CORRECTION_SCALE_NUMERATOR) /
          OSC_DSP_CORRECTION_SCALE_DENOMINATOR) + OSC_DSP_CORRECTION_OFFSET;
  }

  if(endIndex != OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE) return;
  #endif

  if(OSC_DSP_StateMachine.dataAcquisitionState == OSC_DSP_State_Calculating_WaveformConstructPhase){

    calcultaionStatus = OSC_DSP_Waveform_Construct();             /* This function calculates the waveform points and stores them
                                                 in OSC_DisplayManager_Waveform_Channel_A and B structures*/
    if(calcultaionStatus == OSC_DSP_CalculationStatus_Ready){
      OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Calculating_WaveformDisplayPhase;
    }

  } else if(OSC_DSP_StateMachine.dataAcquisitionState == OSC_DSP_State_Calculating_UpdatePhase){

    OSC_DSP_WaveformProperties_Update();      /* This updates the OSC_DSP_WaveformProperties in accordance with the actual configuration*/
    OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Calculating_WaveformConstructPhase;

  } else if(OSC_DSP_StateMachine.dataAcquisitionState == OSC_DSP_State_Calculating_WaveformDisplayPhase){

    OSC_DisplayManager_Graphics_UpdateWaveform(&OSC_DisplayManager_Waveform_Channel_A);   /*These function writes the waveform on the screen*/
    OSC_DisplayManager_Graphics_UpdateWaveform(&OSC_DisplayManager_Waveform_Channel_B);
    OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Disabled;
  }

}

/*
 * Horizontal resolution calculations
 *    samplePerPixel = (sampleRate * timePerDivision) / pixelPerDivision
 *    sampleCount    = samplePerPixel * pixelCount
 */

/*
 * Vertical resolution calculations
 *    The sample values should be converted into mV and this should be processed
 *    voltagePerPixel = voltagePerDivision / pixelPerDivision)
 *    valueInPixel    = (sampleValue * voltagePerLSB) / voltagePerPixel =
 *                    = (sampleValue * voltagePerLSB * pixelPerDivision) / (voltagePerDivision)
 */

void OSC_DSP_WaveformProperties_Update(void){
  int32_t   sampleRate;
  int32_t   timePerDivision;
  int32_t   horizontalOffsetInPixel;
  int32_t   voltagePerLSB;
  int32_t   voltagePerDivision;

  sampleRate       = OSC_Settings_SampleRate.valueSet[OSC_Settings_SampleRate.currentIndex];                      /*sample/ms*/
  timePerDivision  = OSC_Settings_HorizontalResolution.valueSet[OSC_Settings_HorizontalResolution.currentIndex];  /*us/div*/

  OSC_DSP_WaveformProperties.samplePerPixel = (sampleRate * timePerDivision) / (1000 * OSC_DSP_WAVEFORM_PIXEL_PER_HORIZONTAL_DIVISION);
  /*division by 1000 is because of the different units (ms and us)*/

  if(OSC_Settings_DataAcquisitionMode.status == OSC_CFG_DATA_ACQUISITION_MODE_SINGLE){
    horizontalOffsetInPixel = OSC_Settings_HorizontalOffset.value;

    OSC_DSP_WaveformProperties.virtualTriggerPositionInData =
        OSC_DSP_StateMachine.triggerPosition + (OSC_DSP_WaveformProperties.samplePerPixel * horizontalOffsetInPixel);

    /*If the virtual trigger position runs out of the data acquisition memory then it must be saturated to the extremes*/
    if(OSC_DSP_WaveformProperties.virtualTriggerPositionInData < 0){
      OSC_DSP_WaveformProperties.virtualTriggerPositionInData = 0;
    } else if(OSC_DSP_WaveformProperties.virtualTriggerPositionInData >= OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE){
      OSC_DSP_WaveformProperties.virtualTriggerPositionInData = OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE - 1;
    }
  } else {    /*OSC_Settings_DataAcquisitionMode.status == OSC_CFG_DATA_ACQUISITION_MODE_REPETITIVE*/
    OSC_DSP_WaveformProperties.virtualTriggerPositionInData = OSC_DSP_StateMachine.triggerPosition;
  }

  OSC_DSP_WaveformProperties.triggerPositionOnDisplay = (OSC_Settings_TriggerPosition.value * OSC_DSP_WAVEFORM_HORIZONTAL_POINTS_COUNT) /
        (OSC_Settings_TriggerPosition.upperBound - OSC_Settings_TriggerPosition.lowerBound);

  voltagePerLSB      = OSC_Settings_VoltagePerLSB.value;
  voltagePerDivision = OSC_Settings_VerticalResolution.valueSet[OSC_Settings_VerticalResolution.currentIndex];

  OSC_DSP_WaveformProperties.verticalOffsetIn_mV = OSC_Settings_VerticalOffset.value;

  OSC_DSP_WaveformProperties.verticalScaleFactorNumerator   = voltagePerLSB * OSC_DSP_WAVEFORM_PIXEL_PER_VERTICAL_DIVISION;
  OSC_DSP_WaveformProperties.verticalScaleFactorDenominator = voltagePerDivision;
  OSC_DSP_WaveformProperties.verticalOffsetIn_mV            = (OSC_DSP_WaveformProperties.verticalOffsetIn_mV * voltagePerDivision) / OSC_DSP_WAVEFORM_PIXEL_PER_VERTICAL_DIVISION;

  switch(OSC_Settings_DataProcessingMode.optionID){
    case OSC_CFG_DATA_PROCESSING_MODE_NORMAL:
      OSC_DSP_WaveformProperties.dataProcessingMode = OSC_DSP_DataProcessingMode_Normal;
      break;

    case OSC_CFG_DATA_PROCESSING_MODE_AVERAGE:
      OSC_DSP_WaveformProperties.dataProcessingMode = OSC_DSP_DataProcessingMode_Average;
      break;

    case OSC_CFG_DATA_PROCESSING_MODE_PEAK:
      OSC_DSP_WaveformProperties.dataProcessingMode = OSC_DSP_DataProcessingMode_Peak;
      break;
  }
}

OSC_DSP_CalculationStatus_Type OSC_DSP_Waveform_Construct(void){
  static int32_t displayIndex = 0;
  int32_t dataMemoryStartIndex;
  int32_t dataMemoryEndIndex;   /*The first index which is NOT part of the block*/
  int32_t dataLength;
  int32_t dataValue;

  for (; displayIndex < OSC_DSP_WAVEFORM_HORIZONTAL_POINTS_COUNT; ++displayIndex) {

    dataMemoryStartIndex = OSC_DSP_StateMachine.triggerPosition +
                     (displayIndex - OSC_DSP_WaveformProperties.triggerPositionOnDisplay) *
                      OSC_DSP_WaveformProperties.samplePerPixel;

    dataMemoryEndIndex = dataMemoryStartIndex + OSC_DSP_WaveformProperties.samplePerPixel;

    if((OSC_DSP_DATA_MEMORY_INDEX_MAPPER(dataMemoryStartIndex) <  OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE) &&
       (OSC_DSP_DATA_MEMORY_INDEX_MAPPER(dataMemoryEndIndex)   >  0))
    {
      /*In this branch there are enough sample to process them*/
      if(OSC_DSP_DATA_MEMORY_INDEX_MAPPER(dataMemoryEndIndex) >= OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE){
        dataMemoryEndIndex = OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;
        dataLength = dataMemoryEndIndex - dataMemoryStartIndex;
      }

      if(OSC_DSP_DATA_MEMORY_INDEX_MAPPER(dataMemoryStartIndex) < 0){
        dataMemoryStartIndex = 0;
        dataLength = dataMemoryEndIndex - dataMemoryStartIndex;
      }

      dataValue = OSC_DSP_Waveform_CalculateSampleValue(
          OSC_DSP_Channel_A,
          dataMemoryStartIndex % OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE,
          dataLength,
          OSC_DSP_WaveformProperties.dataProcessingMode
      );

      if(OSC_DSP_WaveformProperties.dataProcessingMode == OSC_DSP_DataProcessingMode_Peak){
        OSC_DisplayManager_Waveform_Channel_A.dataPoints[0][displayIndex] = (dataValue >> 16) & 0xFFFF;
        OSC_DisplayManager_Waveform_Channel_A.dataPoints[1][displayIndex] = dataValue & 0xFFFF;
        OSC_DisplayManager_Waveform_Channel_A.dataType = OSC_DisplayManager_Waveform_DataType_MinMax;
      } else {  /*OSC_DSP_WaveformProperties.dataProcessingMode can be OSC_DSP_DataProcessingMode_Normal and OSC_DSP_DataProcessingMode_Average*/
        OSC_DisplayManager_Waveform_Channel_A.dataPoints[0][displayIndex] = dataValue;
        OSC_DisplayManager_Waveform_Channel_A.dataType = OSC_DisplayManager_Waveform_DataType_Normal;
      }

      dataValue = OSC_DSP_Waveform_CalculateSampleValue(
          OSC_DSP_Channel_B,
          dataMemoryStartIndex % OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE,
          dataLength,
          OSC_DSP_WaveformProperties.dataProcessingMode
      );

      if(OSC_DSP_WaveformProperties.dataProcessingMode == OSC_DSP_DataProcessingMode_Peak){
        OSC_DisplayManager_Waveform_Channel_B.dataPoints[0][displayIndex] = (dataValue >> 16) & 0xFFFF;
        OSC_DisplayManager_Waveform_Channel_B.dataPoints[1][displayIndex] = dataValue & 0xFFFF;
        OSC_DisplayManager_Waveform_Channel_B.dataType = OSC_DisplayManager_Waveform_DataType_MinMax;
      } else {  /*OSC_DSP_WaveformProperties.dataProcessingMode can be OSC_DSP_DataProcessingMode_Normal and OSC_DSP_DataProcessingMode_Average*/
        OSC_DisplayManager_Waveform_Channel_B.dataPoints[0][displayIndex] = dataValue;
        OSC_DisplayManager_Waveform_Channel_B.dataType = OSC_DisplayManager_Waveform_DataType_Normal;
      }
    } else {    /*OSC_DSP_DATA_MEMORY_INDEX_MAPPER(index) >= OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE*/
      /*In this branch there are not enough sample to process them*/
      OSC_DisplayManager_Waveform_Channel_A.dataPoints[0][displayIndex] = OSC_DSP_INVALID_DATA_VALUE;
      OSC_DisplayManager_Waveform_Channel_B.dataPoints[0][displayIndex] = OSC_DSP_INVALID_DATA_VALUE;
    }

    if(displayIndex < OSC_DSP_WAVEFORM_HORIZONTAL_POINTS_COUNT){
      return OSC_DSP_CalculationStatus_InProgress;
    } else {
      displayIndex = 0;
      return OSC_DSP_CalculationStatus_Ready;
    }
  }
  return OSC_DSP_CalculationStatus_InProgress;
}

int32_t OSC_DSP_Waveform_VerticalAdjust(int32_t rawData){
  int32_t data;
  data = ((OSC_DSP_WaveformProperties.verticalScaleFactorNumerator * rawData) / OSC_DSP_WaveformProperties.verticalScaleFactorDenominator) +
           OSC_DSP_WaveformProperties.verticalOffsetInPixel;
  if(data < 0){
    data = 0;
  } else if(data >= OSC_DSP_WAVEFORM_VERTICAL_POINTS_COUNT){
    data = OSC_DSP_WAVEFORM_VERTICAL_POINTS_COUNT - 1;
  }
  return data;
}

uint32_t OSC_DSP_Waveform_CalculateSampleValue(
    OSC_DSP_Channel_Type              channel,
    int32_t                           startIndex,
    int32_t                           dataLength,
    OSC_DSP_DataProcessingMode_Type   dataProcMode
){
  int32_t         processedDataValue;
  int32_t         minValue,maxValue;
  int32_t         index, endIndex;  /*endIndex is the last index which is the PART of the block*/
  const uint8_t*  data;

  if(channel == OSC_DSP_Channel_A){
    data = OSC_DSP_Channel_A_DataAcquisitionMemory;
  } else if(channel == OSC_DSP_Channel_B) {
    data = OSC_DSP_Channel_B_DataAcquisitionMemory;
  } else {
    data = NULL;
  }

  if((data == NULL)   || (startIndex >= OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE) || (dataLength > OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE) ||
     (startIndex < 0) || (dataLength < 1)){
    processedDataValue = OSC_DSP_INVALID_DATA_VALUE;
  } else {
    switch(dataProcMode){
    case OSC_DSP_DataProcessingMode_Normal:
      processedDataValue = data[startIndex];
      break;

    case OSC_DSP_DataProcessingMode_Average:
      processedDataValue = 0;
      endIndex = startIndex + dataLength - 1;

      if(endIndex < OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE){
        for (index = startIndex; index < endIndex; ++index) {
          processedDataValue += data[index];
        }
      } else {
        for (index = startIndex; index < OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE; ++index) {
          processedDataValue += data[index];
        }
        for (index = 0; index <= (endIndex - OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE); ++index) {
          processedDataValue += data[index];
        }
      }
      processedDataValue = processedDataValue / dataLength;
      break;

    case OSC_DSP_DataProcessingMode_Peak:
      processedDataValue = 0;
      minValue = 0xFFFF;
      maxValue = 0;
      endIndex = startIndex + dataLength - 1;

      if(endIndex < OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE){
        for (index = startIndex; index < endIndex; ++index) {
          if(data[index] > maxValue) maxValue = data[index];
          if(data[index] < minValue) minValue = data[index];
        }
      } else {
        for (index = startIndex; index < OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE; ++index) {
          if(data[index] > maxValue) maxValue = data[index];
          if(data[index] < minValue) minValue = data[index];
        }
        for (index = 0; index <= (endIndex - OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE); ++index) {
          if(data[index] > maxValue) maxValue = data[index];
          if(data[index] < minValue) minValue = data[index];
        }
      }
      processedDataValue = (maxValue << 16) | minValue;
      break;

    default:
      processedDataValue = OSC_DSP_INVALID_DATA_VALUE;
      break;
    }
  }
  return processedDataValue;
}

void OSC_DSP_StateMachine_Update(void){    /*Updates the DSP state machine configuration dependent attributes*/
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

  if(DMA_GetFlagStatus(OSC_ANALOG_CHANNEL_A_DMA_STREAM,OSC_ANALOG_CHANNEL_A_DMA_STATUS_REGISTER_FLAG_TC)){

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

          DMA_ITConfig(OSC_ANALOG_CHANNEL_A_DMA_STREAM,OSC_ANALOG_CHANNEL_A_DMA_STATUS_REGISTER_ITFLAG_TC,DISABLE);
            /* In OSC_DSP_State_Sampling_Circular_PreTriggerMemory state the transfer complete interrupt is not important because
             * the ADC's analog watchdog interrupt will determine the end of the preTrigger data acquisition ( +DMA in circular mode )*/
        break;

      case OSC_DSP_State_Sampling_Single_PostTriggerMemory_Overflow:
        OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Sampling_Single_PostTriggerMemory_NoOverflow;
        OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger.dataAcquisitionMemory   =  &OSC_DSP_Channel_A_DataAcquisitionMemory[0];
        OSC_Analog_Channel_B_DataAcquisitionConfig_Normal_PostTrigger.dataAcquisitionMemory   =  &OSC_DSP_Channel_A_DataAcquisitionMemory[0];
        OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger.dataLength              =  OSC_DSP_StateMachine.firstDataPosition;  /*because array index starts with 0*/
        OSC_Analog_Channel_B_DataAcquisitionConfig_Normal_PostTrigger.dataLength              =  OSC_DSP_StateMachine.firstDataPosition;  /*because array index starts with 0*/
        OSC_Analog_DMA_ReConfigureBothChannelOnTheFly(
            &OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger,
            &OSC_Analog_Channel_B_DataAcquisitionConfig_Normal_PostTrigger
        );
        break;
      case OSC_DSP_State_Sampling_Single_PostTriggerMemory_NoOverflow:
        OSC_Analog_Conversion_Stop(OSC_Analog_ChannelSelect_ChannelBoth);
        OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Calculating_UpdatePhase;
        break;
      default: break;
    } /*END:switch(OSC_DSP_StateMachine.dataAcquisitionState)*/
    DMA_ClearITPendingBit(OSC_ANALOG_CHANNEL_A_DMA_STREAM,OSC_ANALOG_CHANNEL_A_DMA_STATUS_REGISTER_ITFLAG_TC);
  } /*END: Transfer completed flag is set*/
}

void OSC_ANALOG_ADC_INTERRUPT_HANDLER(void){  /*There is one interrupt for all of the ADCs*/
  ADC_TypeDef* triggerSourceADC = OSC_ANALOG_CHANNEL_A_ADC;
  uint32_t initializedDataLength;
  uint32_t dataLengthRegisterDMA;
  uint32_t fistPostTriggerDataPosition;

  switch(OSC_DSP_StateMachine.triggerState){

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
       * This interrupt is generated immediately after a conversion (new value -> analog watchdog check -> interrupt)
       *
       * This interrupt should be finished in 2 ADC conversion time because the trigger position value is passed to the
       * DMA before the deactivation so when this interrupt starts the data register is empty. After the next conversion
       * is ready a DMA request is sent and this must be serviced before the next conversion in order to avoid overflow
       * Time constraint: 2 * 168 = 336 clock cycle (168MHz and 1Msample/s)
       */
      OSC_ANALOG_CHANNEL_A_DMA_STREAM_STOP();
      OSC_ANALOG_CHANNEL_B_DMA_STREAM_STOP();

      switch(OSC_DSP_StateMachine.triggerSource){
        case OSC_DSP_TriggerSource_Channel_A:
          dataLengthRegisterDMA   = OSC_ANALOG_CHANNEL_A_DMA_STREAM_DATA_LENGTH_GET();
          initializedDataLength   = OSC_Analog_Channel_A_DataAcquisitionConfig_Circular_PreTrigger.dataLength;
          break;
        case OSC_DSP_TriggerSource_Channel_B:
          dataLengthRegisterDMA   = OSC_ANALOG_CHANNEL_B_DMA_STREAM_DATA_LENGTH_GET();
          initializedDataLength = OSC_Analog_Channel_B_DataAcquisitionConfig_Circular_PreTrigger.dataLength;
          break;
        default:
          break;
      }

      OSC_DSP_StateMachine.triggerPosition   = initializedDataLength - dataLengthRegisterDMA;   /*It can't be negative*/
      OSC_DSP_StateMachine.firstDataPosition = OSC_DSP_StateMachine.triggerPosition + OSC_DSP_StateMachine.postTriggerMemoryLength;
      fistPostTriggerDataPosition = ((OSC_DSP_StateMachine.triggerPosition + 1) > OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE) ?
                                     (OSC_DSP_StateMachine.triggerPosition + 1) : 0;

      if(OSC_DSP_StateMachine.postTriggerMemoryLength > 1){ /*triggerPosition data is the part of postTrigger memory*/
        OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger.dataAcquisitionMemory =
            &OSC_DSP_Channel_A_DataAcquisitionMemory[fistPostTriggerDataPosition];
        OSC_Analog_Channel_B_DataAcquisitionConfig_Normal_PostTrigger.dataAcquisitionMemory =
            &OSC_DSP_Channel_B_DataAcquisitionMemory[fistPostTriggerDataPosition];

        if(OSC_DSP_StateMachine.firstDataPosition < OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE){
          OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Sampling_Single_PostTriggerMemory_NoOverflow;
          OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger.dataLength =
              OSC_DSP_StateMachine.firstDataPosition - fistPostTriggerDataPosition;   /*triggerPosition data has already been transmitted*/
        } else {  /*OSC_DSP_StateMachine.firstDataPosition >= OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE*/
          OSC_DSP_StateMachine.firstDataPosition   -= OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;
          OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger.dataLength =
              OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE - fistPostTriggerDataPosition;     /*triggerPosition data has already been transmitted*/
          OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Sampling_Single_PostTriggerMemory_Overflow;
        }

        OSC_Analog_Channel_B_DataAcquisitionConfig_Normal_PostTrigger.dataLength = OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger.dataLength;
        /*The two channel has to be synchronized so the data length for the DMA must be the same*/

        OSC_Analog_DMA_ReConfigureBothChannelOnTheFly(      /*dmaMode in the structure is set at the beginning of the program and it doesn't change*/
                    &OSC_Analog_Channel_A_DataAcquisitionConfig_Normal_PostTrigger,
                    &OSC_Analog_Channel_B_DataAcquisitionConfig_Normal_PostTrigger
                );
        /*At this point it must be in 336 clock cycle since the start of the interrupt*/
      } else {  /*OSC_DSP_StateMachine.postTriggerMemoryLength <= 1*/
        /*The condition contains 1 instead of 0 because the triggerPosition data is the part of the postTrigger data*/
        OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Calculating_UpdatePhase;
        /*if there are no need for postTrigger data acquisition then the next phase is the calculating*/
      }   /*END:OSC_DSP_StateMachine.postTriggerMemoryLength <= 1*/
      OSC_DSP_StateMachine.triggerAnalogWatchdogRange = OSC_Analog_AnalogWatchdog_Range_Invalid;
      OSC_DSP_StateMachine.triggerState               = OSC_DSP_TriggerState_Disabled;
      DMA_ITConfig(OSC_ANALOG_CHANNEL_A_DMA_STREAM,OSC_ANALOG_CHANNEL_A_DMA_STATUS_REGISTER_ITFLAG_TC,ENABLE);
      OSC_Analog_AnalogWatchdog_Disable();
    }   /*END:OSC_DSP_StateMachine.dataAcquisitionState == OSC_DSP_State_Sampling_Circular_PreTriggerMemory*/
  }   /*END:OSC_DSP_StateMachine.triggerState == OSC_DSP_TriggerState_Enabled_Active*/

  ADC_ClearITPendingBit(triggerSourceADC, ADC_IT_AWD);
}
