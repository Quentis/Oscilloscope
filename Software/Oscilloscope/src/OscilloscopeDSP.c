#include "OscilloscopeDSP.h"

/*=================================== DATA ACQUISITION MEMORY DEFINITIONS ===================================*/
uint8_t OSC_DSP_Channel_A_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE] __attribute__ ((aligned (0x1000)));
uint8_t OSC_DSP_Channel_B_DataAcquisitionMemory[OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE] __attribute__ ((aligned (0x1000)));

/*===================================== DMA MODE STRUCTURE DEFINITIONS ======================================*/
static const OSC_Analog_Channel_DataAcquisitionConfig_Type OSC_Analog_Channel_A_DataAcquisitionConfig = {
    OSC_DSP_Channel_A_DataAcquisitionMemory,
    OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE,
    OSC_Analog_DMA_Mode_Circular
};

static const OSC_Analog_Channel_DataAcquisitionConfig_Type OSC_Analog_Channel_B_DataAcquisitionConfig = {
    OSC_DSP_Channel_B_DataAcquisitionMemory,
    OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE,
    OSC_Analog_DMA_Mode_Circular
};

/*======================================== STATE MACHINE DEFINITIONS ========================================*/
OSC_DSP_StateMachine_Type  OSC_DSP_StateMachine = {
    OSC_DSP_State_Idle,                             /*dataAcquisitionState*/              /*COMPUTED*/
    0,                                              /*firstDataPosition*/                 /*COMPUTED*/
    0,                                              /*preTriggerMemoryLength*/            /*UPDATE-FROM-CONFIG*/
    0,                                              /*postTriggerMemoryLength*/           /*UPDATE-FROM-CONFIG*/
    0,                                              /*triggerPosition*/                   /*COMPUTED*/
    127,                                            /*triggerLevel*/                      /*UPDATE-FROM-CONFIG*/
    OSC_Analog_AnalogWatchdog_Range_Invalid,        /*triggerAnalogWatchdogRange*/        /*COMPUTED*/
    OSC_DSP_TriggerSlope_RisingEdge,                /*triggerSlope*/                      /*UPDATE-FROM-CONFIG*/
    OSC_DSP_TriggerSource_Channel_A,                /*triggerSource*/                     /*UPDATE-FROM-CONFIG*/
    OSC_DSP_TriggerState_Disabled,                  /*triggerState*/                      /*COMPUTED*/
    OSC_DSP_SAMPLE_RATE,                            /*sampleRate*/                        /*UPDATE-FROM-CONFIG*/
    OSC_DSP_DataAcquisitionMode_Repetitive          /*dataAcquisitionMode*/               /*UPDATE-FROM-CONFIG*/
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
    OSC_DSP_DataProcessingMode_Normal,                 /*dataProcessingMode*/
    0                                                  /*waveformMemoryIndex*/
};

/*====================================== EXTERNAL FUNCTION DEFINITIONS ======================================*/

void OSC_DSP_Init(void){

}

OSC_DSP_DataAcquisitionMode_Type OSC_DSP_GetDataAcquisitionMode(void){
  if(OSC_Settings_DataProcessingMode.optionID == OSC_CFG_DATA_ACQUISITION_MODE_REPETITIVE){
    return OSC_DSP_DataAcquisitionMode_Repetitive;
  } else {  /*OSC_Settings_DataProcessingMode.optionID == OSC_CFG_DATA_ACQUISITION_MODE_SINGLE*/
    return OSC_DSP_DataAcquisitionMode_Single;
  }
}

void OSC_DSP_StartDataAcquisition(void){
  OSC_DSP_StateMachine_Update();
  OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Sampling_PreTriggerMemory_FirstTime;
  OSC_DSP_StateMachine.triggerState = OSC_DSP_TriggerState_Disabled;
  OSC_DSP_StateMachine.triggerAnalogWatchdogRange = OSC_Analog_AnalogWatchdog_Range_Invalid;

  OSC_Analog_DMA_ReConfigureBothChannelOnTheFly(
      &OSC_Analog_Channel_A_DataAcquisitionConfig,
      &OSC_Analog_Channel_B_DataAcquisitionConfig
  );

  OSC_Analog_Conversion_Start(OSC_Analog_ChannelSelect_ChannelBoth);
  DMA_ITConfig(OSC_ANALOG_CHANNEL_A_DMA_STREAM,OSC_ANALOG_CHANNEL_A_DMA_STREAM_INTERRUPT_TC_ENABLE_BIT,ENABLE);
}

void OSC_DSP_ReCalculate(void){
  if(OSC_DSP_StateMachine.dataAcquisitionState == OSC_DSP_State_Idle){
    OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Calculating_UpdatePhase;
    /* OSC_DSP_Calculate(...) is a polling function and it will handle everything it will update the
     * OSC_DSP_WaveformProperties and it will recalculate the waveform and it will display it*/
  }
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

    /*These function writes the waveform on the screen*/
    if(OSC_Settings_Channel_A_Status.status == OSC_CFG_CHANNEL_A_STATUS_ENABLED){
      OSC_DisplayManager_Graphics_UpdateWaveform(&OSC_DisplayManager_Waveform_Channel_A[0]);
    }

    if(OSC_Settings_Channel_B_Status.status == OSC_CFG_CHANNEL_B_STATUS_ENABLED){
      OSC_DisplayManager_Graphics_UpdateWaveform(&OSC_DisplayManager_Waveform_Channel_B[0]);
    }

    OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Idle;
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

  OSC_DSP_WaveformProperties.verticalOffsetInPixel = OSC_Settings_VerticalOffset.value;

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

  OSC_DSP_WaveformProperties.waveformMemoryIndex = (OSC_DSP_WaveformProperties.waveformMemoryIndex == 0) ? 1 : 0;
}

OSC_DSP_CalculationStatus_Type OSC_DSP_Waveform_Construct(void){
  static int32_t displayIndex = 0;
  int32_t dataMemoryStartIndex;
  int32_t dataMemoryEndIndex;   /*The first index which is NOT part of the block*/
  int32_t dataLength, dataValue;
  int32_t dataMemoryStartIndexLogical, dataMemoryEndIndexLogical;
  /* These variables are created from mapping of the similar named variables
   * The mapping follows the principle: first data is at the 0 index and
   * the last data is at the data length minus one index*/

  dataMemoryStartIndex = OSC_DSP_StateMachine.triggerPosition +
                   (displayIndex - OSC_DSP_WaveformProperties.triggerPositionOnDisplay) *
                    OSC_DSP_WaveformProperties.samplePerPixel;

  dataMemoryStartIndexLogical = dataMemoryStartIndex - OSC_DSP_StateMachine.firstDataPosition;
  if(dataMemoryStartIndexLogical < 0) dataMemoryStartIndexLogical += OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;

  dataMemoryEndIndex = dataMemoryStartIndex + OSC_DSP_WaveformProperties.samplePerPixel;

  dataMemoryEndIndexLogical = dataMemoryEndIndex - OSC_DSP_StateMachine.firstDataPosition;
  if(dataMemoryEndIndexLogical < 0) dataMemoryEndIndexLogical += OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;

  if((dataMemoryStartIndexLogical <  OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE) &&
     (dataMemoryEndIndexLogical   >  0))
  {
    /*In this branch there are enough sample to process them*/
    if(dataMemoryStartIndex < 0) dataMemoryStartIndex += OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;
    if(dataMemoryEndIndex   < 0) dataMemoryEndIndex   += OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;

    dataLength = OSC_DSP_WaveformProperties.samplePerPixel;

    if(dataMemoryEndIndexLogical > OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE){
      dataMemoryEndIndex = ((OSC_DSP_StateMachine.firstDataPosition - 1) > 0) ?
                            (OSC_DSP_StateMachine.firstDataPosition - 1) :
                             OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;
      dataMemoryEndIndexLogical = OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;
      dataLength = dataMemoryEndIndexLogical - dataMemoryStartIndexLogical;
    }

    if(dataMemoryStartIndexLogical < 0){
      dataMemoryStartIndex = OSC_DSP_StateMachine.firstDataPosition;
      dataMemoryStartIndexLogical = 0;
      dataLength = dataMemoryEndIndexLogical - dataMemoryStartIndexLogical;
    }

    dataValue = OSC_DSP_Waveform_CalculateSampleValue(
        OSC_DSP_Channel_A,
        dataMemoryStartIndex % OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE,
        dataLength,
        OSC_DSP_WaveformProperties.dataProcessingMode
    );

    if(OSC_DSP_WaveformProperties.dataProcessingMode == OSC_DSP_DataProcessingMode_Peak){
      OSC_DisplayManager_Waveform_Channel_A[OSC_DSP_WaveformProperties.waveformMemoryIndex].dataPoints[0][displayIndex] =
                         OSC_DSP_Waveform_VerticalAdjust((dataValue >> 16) & 0xFFFF);
      OSC_DisplayManager_Waveform_Channel_A[OSC_DSP_WaveformProperties.waveformMemoryIndex].dataPoints[1][displayIndex] =
                         OSC_DSP_Waveform_VerticalAdjust(dataValue & 0xFFFF);
      OSC_DisplayManager_Waveform_Channel_A[OSC_DSP_WaveformProperties.waveformMemoryIndex].dataType = OSC_DisplayManager_Waveform_DataType_MinMax;
    } else {  /*OSC_DSP_WaveformProperties.dataProcessingMode can be OSC_DSP_DataProcessingMode_Normal and OSC_DSP_DataProcessingMode_Average*/
      OSC_DisplayManager_Waveform_Channel_A[OSC_DSP_WaveformProperties.waveformMemoryIndex].dataPoints[0][displayIndex] =
                         OSC_DSP_Waveform_VerticalAdjust(dataValue);
      OSC_DisplayManager_Waveform_Channel_A[OSC_DSP_WaveformProperties.waveformMemoryIndex].dataType =
                         OSC_DisplayManager_Waveform_DataType_Normal;
    }

    dataValue = OSC_DSP_Waveform_CalculateSampleValue(
        OSC_DSP_Channel_B,
        dataMemoryStartIndex % OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE,
        dataLength,
        OSC_DSP_WaveformProperties.dataProcessingMode
    );

    dataValue = OSC_DSP_Waveform_VerticalAdjust(dataValue);

    if(OSC_DSP_WaveformProperties.dataProcessingMode == OSC_DSP_DataProcessingMode_Peak){
      OSC_DisplayManager_Waveform_Channel_B[OSC_DSP_WaveformProperties.waveformMemoryIndex].dataPoints[0][displayIndex] =
                         OSC_DSP_Waveform_VerticalAdjust((dataValue >> 16) & 0xFFFF);
      OSC_DisplayManager_Waveform_Channel_B[OSC_DSP_WaveformProperties.waveformMemoryIndex].dataPoints[1][displayIndex] =
                         OSC_DSP_Waveform_VerticalAdjust(dataValue & 0xFFFF);
      OSC_DisplayManager_Waveform_Channel_B[OSC_DSP_WaveformProperties.waveformMemoryIndex].dataType = OSC_DisplayManager_Waveform_DataType_MinMax;
    } else {  /*OSC_DSP_WaveformProperties.dataProcessingMode can be OSC_DSP_DataProcessingMode_Normal and OSC_DSP_DataProcessingMode_Average*/
      OSC_DisplayManager_Waveform_Channel_B[OSC_DSP_WaveformProperties.waveformMemoryIndex].dataPoints[0][displayIndex] =
                         OSC_DSP_Waveform_VerticalAdjust(dataValue);
      OSC_DisplayManager_Waveform_Channel_B[OSC_DSP_WaveformProperties.waveformMemoryIndex].dataType =
                         OSC_DisplayManager_Waveform_DataType_Normal;
    }
  } else {    /*OSC_DSP_DATA_MEMORY_INDEX_MAPPER(index) >= OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE*/
    /*In this branch there are not enough sample to process them*/
    OSC_DisplayManager_Waveform_Channel_A[OSC_DSP_WaveformProperties.waveformMemoryIndex].dataPoints[0][displayIndex] =
        OSC_DSP_INVALID_DATA_VALUE;
    OSC_DisplayManager_Waveform_Channel_B[OSC_DSP_WaveformProperties.waveformMemoryIndex].dataPoints[0][displayIndex] =
        OSC_DSP_INVALID_DATA_VALUE;
  }
  displayIndex++;

  if(displayIndex < OSC_DSP_WAVEFORM_HORIZONTAL_POINTS_COUNT){
    return OSC_DSP_CalculationStatus_InProgress;
  } else {
    displayIndex = 0;
    return OSC_DSP_CalculationStatus_Ready;
  }
}

int32_t OSC_DSP_Waveform_VerticalAdjust(int32_t rawData){
  int32_t data;
  data = ((OSC_DSP_WaveformProperties.verticalScaleFactorNumerator * (rawData - OSC_DSP_RAW_DATA_ZERO_VALUE)) / OSC_DSP_WaveformProperties.verticalScaleFactorDenominator) +
           OSC_DSP_WaveformProperties.verticalOffsetInPixel + OSC_DSP_ZERO_DATA_VERTICAL_PIXEL_COUNT;
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
  OSC_DSP_StateMachine.dataAcquisitionState           = OSC_DSP_State_Idle;

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

  if(OSC_Settings_DataProcessingMode.optionID == OSC_CFG_DATA_ACQUISITION_MODE_REPETITIVE){
    OSC_DSP_StateMachine.dataAcquisitionMode = OSC_DSP_DataAcquisitionMode_Repetitive;
  } else {  /*OSC_Settings_DataProcessingMode.optionID == OSC_CFG_DATA_ACQUISITION_MODE_SINGLE*/
    OSC_DSP_StateMachine.dataAcquisitionMode = OSC_DSP_DataAcquisitionMode_Single;
  }
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

    while(!(OSC_ANALOG_CHANNEL_B_DMA_STATUS_REGISTER & OSC_ANALOG_CHANNEL_B_DMA_STATUS_REGISTER_FLAG_TC));

    if(OSC_DSP_StateMachine.dataAcquisitionState == OSC_DSP_State_Sampling_PreTriggerMemory_FirstTime){

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

      OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Sampling_WaitingForTrigger;
      DMA_ITConfig(OSC_ANALOG_CHANNEL_A_DMA_STREAM,OSC_ANALOG_CHANNEL_A_DMA_STREAM_INTERRUPT_TC_ENABLE_BIT,DISABLE);

    } /*END:OSC_DSP_StateMachine.dataAcquisitionState == OSC_DSP_State_Sampling_PreTriggerMemory_FirstTime*/

    DMA_ClearITPendingBit(OSC_ANALOG_CHANNEL_A_DMA_STREAM,OSC_ANALOG_CHANNEL_A_DMA_STATUS_REGISTER_ITFLAG_TC);
    DMA_ClearITPendingBit(OSC_ANALOG_CHANNEL_B_DMA_STREAM,OSC_ANALOG_CHANNEL_B_DMA_STATUS_REGISTER_ITFLAG_TC);

  } /*END: Transfer completed flag is set*/
}

void OSC_ANALOG_ADC_INTERRUPT_HANDLER(void){  /*There is one interrupt for all of the ADCs*/
  ADC_TypeDef* triggerSourceADC = OSC_ANALOG_CHANNEL_A_ADC;
  uint32_t initializedDataLength;
  uint32_t dataLengthRegisterDMA;

  if(OSC_DSP_StateMachine.triggerSource == OSC_DSP_TriggerSource_Channel_A){
    triggerSourceADC = OSC_ANALOG_CHANNEL_A_ADC;
  } else {  /*OSC_DSP_StateMachine.triggerSource == OSC_DSP_TriggerSource_Channel_B*/
    triggerSourceADC = OSC_ANALOG_CHANNEL_B_ADC;
  }

  switch(OSC_DSP_StateMachine.triggerState){

    case OSC_DSP_TriggerState_Enabled_TwoMoreInterrupt:
      OSC_DSP_StateMachine.triggerState = OSC_DSP_TriggerState_Enabled_NextInterrupt;
      OSC_Analog_AnalogWatchdog_Disable();


      if(OSC_DSP_StateMachine.triggerAnalogWatchdogRange == OSC_Analog_AnalogWatchdog_Range_Upper){
        OSC_DSP_StateMachine.triggerAnalogWatchdogRange = OSC_Analog_AnalogWatchdog_Range_Lower;
      } else {    /*OSC_DSP_StateMachine.triggerAnalogWatchdogRange == OSC_Analog_AnalogWatchdog_Range_Lower*/
        OSC_DSP_StateMachine.triggerAnalogWatchdogRange = OSC_Analog_AnalogWatchdog_Range_Upper;
      }
      OSC_Analog_AnalogWatchdog_Enable(triggerSourceADC,OSC_DSP_StateMachine.triggerLevel,OSC_DSP_StateMachine.triggerAnalogWatchdogRange);
      break;

    case OSC_DSP_TriggerState_Enabled_NextInterrupt:
      OSC_DSP_StateMachine.triggerState = OSC_DSP_TriggerState_Enabled_Active;
      TIM_SetCounter(OSC_ANALOG_DATA_COUNTER_TIMER,0);
      TIM_SetAutoreload(OSC_ANALOG_DATA_COUNTER_TIMER,OSC_DSP_StateMachine.postTriggerMemoryLength - 2);
      TIM_Cmd(OSC_ANALOG_DATA_COUNTER_TIMER,ENABLE);
             /* -1 because of the trigger is the part of the postTrigger memory and it has already been written
              *  to the memory and -1 because period ARR = (period - 1) */
      break;

    default:
      OSC_DSP_StateMachine.triggerState = OSC_DSP_TriggerState_Disabled;
      break;
  }

  if(OSC_DSP_StateMachine.triggerState == OSC_DSP_TriggerState_Enabled_Active){
    if(OSC_DSP_StateMachine.dataAcquisitionState == OSC_DSP_State_Sampling_WaitingForTrigger){

      switch(OSC_DSP_StateMachine.triggerSource){
        case OSC_DSP_TriggerSource_Channel_A:
          dataLengthRegisterDMA   = OSC_ANALOG_CHANNEL_A_DMA_STREAM_DATA_LENGTH_GET();
          initializedDataLength   = OSC_Analog_Channel_A_DataAcquisitionConfig.dataLength;
          break;
        case OSC_DSP_TriggerSource_Channel_B:
          dataLengthRegisterDMA   = OSC_ANALOG_CHANNEL_B_DMA_STREAM_DATA_LENGTH_GET();
          initializedDataLength   = OSC_Analog_Channel_B_DataAcquisitionConfig.dataLength;
          break;

        default:
          break;
      }

      OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Sampling_PostTriggerMemory;
      OSC_DSP_StateMachine.triggerPosition   = initializedDataLength - dataLengthRegisterDMA;   /*It can't be negative*/
      OSC_DSP_StateMachine.firstDataPosition = (OSC_DSP_StateMachine.triggerPosition + OSC_DSP_StateMachine.postTriggerMemoryLength) %
                                                OSC_DSP_DATA_ACQUISITION_MEMORY_SIZE;

      OSC_DSP_StateMachine.triggerAnalogWatchdogRange = OSC_Analog_AnalogWatchdog_Range_Invalid;
      OSC_DSP_StateMachine.triggerState               = OSC_DSP_TriggerState_Disabled;

      OSC_Analog_AnalogWatchdog_Disable();

    }   /*END:OSC_DSP_StateMachine.dataAcquisitionState == OSC_DSP_State_Sampling_Circular_PreTriggerMemory*/
  }   /*END:OSC_DSP_StateMachine.triggerState == OSC_DSP_State_Sampling_WaitingForTrigger*/

  ADC_ClearITPendingBit(triggerSourceADC, ADC_IT_AWD);
}

void OSC_ANALOG_DATA_COUNTER_TIMER_INTERRUPT_HANDLER(void){
  if(TIM_GetITStatus(OSC_ANALOG_DATA_COUNTER_TIMER,TIM_IT_Update)){
    OSC_Analog_Conversion_Stop(OSC_Analog_ChannelSelect_ChannelBoth);
    TIM_Cmd(OSC_ANALOG_DATA_COUNTER_TIMER,DISABLE);
    OSC_DSP_StateMachine.dataAcquisitionState = OSC_DSP_State_Calculating_UpdatePhase;
    TIM_ClearITPendingBit(OSC_ANALOG_DATA_COUNTER_TIMER,TIM_IT_Update);
  }
}
