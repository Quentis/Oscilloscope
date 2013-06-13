#include "OscilloscopeAnalog.h"

void OSC_Analog_Init_DMA(void){
  NVIC_InitTypeDef NVIC_InitStructure = {
      DMA2_Stream0_IRQn,
      OSC_ANALOG_ADC_PREEMP_PRIO,
      OSC_ANALOG_ADC_SUBPRIO,
      ENABLE
  }; /*DMA Transfer completed interrupt*/

  DMA_InitTypeDef DMA_InitStructure_Channel_A = {
    OSC_ANALOG_CHANNEL_A_DMA_CHANNEL,
    OSC_ANALOG_CHANNEL_A_DMA_PERIPH_BASE_ADDRESS,
    OSC_ANALOG_CHANNEL_A_DMA_MEMORY0_BASE_ADDRESS,
    OSC_ANALOG_CHANNEL_A_DMA_DIR,
    OSC_ANALOG_CHANNEL_A_DMA_BUFFER_SIZE,
    OSC_ANALOG_CHANNEL_A_DMA_PERIPH_ADDRESS_INC,
    OSC_ANALOG_CHANNEL_A_DMA_MEMORY_ADDRESS_INC,
    OSC_ANALOG_CHANNEL_A_DMA_PERIPH_DATA_SIZE,
    OSC_ANALOG_CHANNEL_A_DMA_MEMORY_DATA_SIZE,
    OSC_ANALOG_CHANNEL_A_DMA_MODE,
    OSC_ANALOG_CHANNEL_A_DMA_PRIO,
    OSC_ANALOG_CHANNEL_A_DMA_FIFO_MODE,
    OSC_ANALOG_CHANNEL_A_DMA_FIFO_THRESHOLD,
    OSC_ANALOG_CHANNEL_A_DMA_MEMORY_BURST,
    OSC_ANALOG_CHANNEL_A_DMA_PERIPH_BURST
  };

  DMA_InitTypeDef DMA_InitStructure_Channel_B = {
    OSC_ANALOG_CHANNEL_B_DMA_CHANNEL,
    OSC_ANALOG_CHANNEL_B_DMA_PERIPH_BASE_ADDRESS,
    OSC_ANALOG_CHANNEL_B_DMA_MEMORY0_BASE_ADDRESS,
    OSC_ANALOG_CHANNEL_B_DMA_DIR,
    OSC_ANALOG_CHANNEL_B_DMA_BUFFER_SIZE,
    OSC_ANALOG_CHANNEL_B_DMA_PERIPH_ADDRESS_INC,
    OSC_ANALOG_CHANNEL_B_DMA_MEMORY_ADDRESS_INC,
    OSC_ANALOG_CHANNEL_B_DMA_PERIPH_DATA_SIZE,
    OSC_ANALOG_CHANNEL_B_DMA_MEMORY_DATA_SIZE,
    OSC_ANALOG_CHANNEL_B_DMA_MODE,
    OSC_ANALOG_CHANNEL_B_DMA_PRIO,
    OSC_ANALOG_CHANNEL_B_DMA_FIFO_MODE,
    OSC_ANALOG_CHANNEL_B_DMA_FIFO_THRESHOLD,
    OSC_ANALOG_CHANNEL_B_DMA_MEMORY_BURST,
    OSC_ANALOG_CHANNEL_B_DMA_PERIPH_BURST
  };

  RCC_AHB1PeriphClockCmd(OSC_ANALOG_CHANNEL_A_DMA_CLK,ENABLE);
  RCC_AHB1PeriphClockCmd(OSC_ANALOG_CHANNEL_B_DMA_CLK,ENABLE);

  DMA_Init(OSC_ANALOG_CHANNEL_A_DMA_STREAM,&DMA_InitStructure_Channel_A);
  DMA_Init(OSC_ANALOG_CHANNEL_B_DMA_STREAM,&DMA_InitStructure_Channel_B);

  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;
  NVIC_Init(&NVIC_InitStructure);

  DMA_Cmd(OSC_ANALOG_CHANNEL_A_DMA_STREAM,ENABLE);
  DMA_Cmd(OSC_ANALOG_CHANNEL_B_DMA_STREAM,ENABLE);
}

void OSC_Analog_Init_DAC(void){
  DAC_InitTypeDef DAC_InitStructure = {
      DAC_Trigger_None,
      DAC_WaveGeneration_None,
      DAC_TriangleAmplitude_1,  /*Dummy value*/
      DAC_OutputBuffer_Enable
  };

  GPIO_InitTypeDef GPIO_InitStructure = {
      0,
      GPIO_Mode_AN,
      GPIO_Speed_100MHz,
      GPIO_OType_OD,    /*NoEffect*/
      GPIO_PuPd_NOPULL  /*NoEffect*/
  };

  /*DAC initialization*/
  OSC_ANALOG_DAC_CLK_ENABLE(OSC_ANALOG_DAC_CLK,ENABLE);
  RCC_AHB1PeriphClockCmd(OSC_ANALOG_DAC_GPIO_CLK,ENABLE);

  GPIO_InitStructure.GPIO_Pin = OSC_ANALOG_DAC_GPIO_PIN;
  GPIO_Init(OSC_ANALOG_DAC_GPIO_PORT,&GPIO_InitStructure);
  DAC_Init(OSC_ANALOG_DAC_CHANNEL,&DAC_InitStructure);

  DAC_SetChannel1Data(DAC_Align_12b_R,OSC_ANALOG_DAC_OUPUT_VALUE);
  DAC_Cmd(OSC_ANALOG_DAC_CHANNEL,ENABLE);
}

void OSC_Analog_Init_ADC(void){
  ADC_InitTypeDef ADC_InitStructure = {
      ADC_Resolution_8b,
      OSC_ANALOG_ADC_SCAN_DISABLED,
      OSC_ANALOG_ADC_CONTINUOUSMODE_DISABLED,
      ADC_ExternalTrigConvEdge_Rising,
      ADC_ExternalTrigConv_T8_CC1,
      ADC_DataAlign_Right,
      OSC_ANALOG_ADC_NUM_OF_CONV
  };
  ADC_CommonInitTypeDef ADC_Common_InitStructure = {
      ADC_Mode_Independent,
      OSC_ANALOG_ADC_PRESCALER_FOR_21MHZ,
      ADC_DMAAccessMode_Disabled,
      ADC_TwoSamplingDelay_5Cycles
  };

  GPIO_InitTypeDef GPIO_InitStructure = {
      0,
      GPIO_Mode_AN,
      GPIO_Speed_100MHz,
      GPIO_OType_OD,    /*NoEffect*/
      GPIO_PuPd_NOPULL  /*NoEffect*/
  };

  /*ADC initialization*/
  RCC_AHB1PeriphClockCmd(OSC_ANALOG_CHANNEL_A_ADC_GPIO_CLK | OSC_ANALOG_CHANNEL_B_ADC_GPIO_CLK,ENABLE);
  OSC_ANALOG_ADC_CLK_ENABLE(OSC_ANALOG_CHANNEL_A_ADC_CLK | OSC_ANALOG_CHANNEL_B_ADC_CLK, ENABLE);

  GPIO_InitStructure.GPIO_Pin = OSC_ANALOG_CHANNEL_A_ADC_GPIO_PIN;
  GPIO_Init(OSC_ANALOG_CHANNEL_A_ADC_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_ANALOG_CHANNEL_B_ADC_GPIO_PIN;
  GPIO_Init(OSC_ANALOG_CHANNEL_B_ADC_GPIO_PORT,&GPIO_InitStructure);

  ADC_Init(OSC_ANALOG_CHANNEL_A_ADC,&ADC_InitStructure);
  ADC_Init(OSC_ANALOG_CHANNEL_B_ADC,&ADC_InitStructure);

  ADC_RegularChannelConfig(OSC_ANALOG_CHANNEL_A_ADC,OSC_ANALOG_CHANNEL_A_ADC_CHANNEL,1,ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(OSC_ANALOG_CHANNEL_B_ADC,OSC_ANALOG_CHANNEL_B_ADC_CHANNEL,1,ADC_SampleTime_3Cycles);

  ADC_CommonInit(&ADC_Common_InitStructure);

  ADC_AnalogWatchdogSingleChannelConfig(OSC_ANALOG_CHANNEL_A_ADC,OSC_ANALOG_CHANNEL_A_ADC_CHANNEL);
  ADC_AnalogWatchdogSingleChannelConfig(OSC_ANALOG_CHANNEL_B_ADC,OSC_ANALOG_CHANNEL_B_ADC_CHANNEL);
  ADC_AnalogWatchdogThresholdsConfig(OSC_ANALOG_CHANNEL_A_ADC,0x7F,0x0);
  ADC_AnalogWatchdogThresholdsConfig(OSC_ANALOG_CHANNEL_B_ADC,0x7F,0x0);

  ADC_DMARequestAfterLastTransferCmd(OSC_ANALOG_CHANNEL_A_ADC,ENABLE);
  ADC_DMACmd(OSC_ANALOG_CHANNEL_A_ADC,ENABLE);

  ADC_DMARequestAfterLastTransferCmd(OSC_ANALOG_CHANNEL_B_ADC,ENABLE);
  ADC_DMACmd(OSC_ANALOG_CHANNEL_B_ADC,ENABLE);
}

void OSC_Analog_Init_Timer(void){
  TIM_TimeBaseInitTypeDef TIM_Base_InitStructure = {
      OSC_ANALOG_SAMPLE_TIMER_PRELOAD,
      TIM_CounterMode_Up,
      OSC_ANALOG_SAMPLE_TIMER_PERIOD,
      OSC_ANALOG_SAMPLE_TIMER_CLOCK_DIV_0,
      OSC_ANALOG_SAMPLE_TIMER_REPETITION_NO
  };

  TIM_OCInitTypeDef TIM_OC_InitStructure = {
      TIM_OCMode_PWM1,
      TIM_OutputState_Disable,
      TIM_OutputNState_Disable,
      OSC_ANALOG_SAMPLE_TIMER_COMPARE_PULSE,
      TIM_OCPolarity_High,
      TIM_OCNPolarity_Low,
      TIM_OCIdleState_Reset,
      TIM_OCNIdleState_Reset
  };

  TIM_BDTRInitTypeDef TIM_BDTR_Initstructure = {
      TIM_OSSRState_Enable,
      TIM_OSSIState_Enable,
      TIM_LOCKLevel_OFF,
      OSC_ANALOG_SAMPLE_TIMER_DEADTIME,
      TIM_Break_Disable,
      TIM_BreakPolarity_Low,
      TIM_AutomaticOutput_Enable
  };

  /*Timer initialization*/
  OSC_ANALOG_SAMPLE_TIMER_CLK_ENABLE(OSC_ANALOG_SAMPLE_TIMER_CLK,ENABLE);

  TIM_TimeBaseInit(OSC_ANALOG_SAMPLE_TIMER,&TIM_Base_InitStructure);
  TIM_OC1Init(OSC_ANALOG_SAMPLE_TIMER,&TIM_OC_InitStructure);
  TIM_BDTRConfig(OSC_ANALOG_SAMPLE_TIMER,&TIM_BDTR_Initstructure);

  TIM_ARRPreloadConfig(OSC_ANALOG_SAMPLE_TIMER,ENABLE);
  TIM_OC1PreloadConfig(OSC_ANALOG_SAMPLE_TIMER,TIM_OCPreload_Enable);

  TIM_GenerateEvent(OSC_ANALOG_SAMPLE_TIMER,TIM_EventSource_COM);
  TIM_GenerateEvent(OSC_ANALOG_SAMPLE_TIMER,TIM_EventSource_Update);

  TIM_CCxCmd(OSC_ANALOG_SAMPLE_TIMER,TIM_Channel_1,TIM_CCx_Enable);
  TIM_Cmd(OSC_ANALOG_SAMPLE_TIMER,ENABLE);
}

void OSC_Analog_Init_1kHzSquareWave(void){
  GPIO_InitTypeDef GPIO_InitStructure = {
      OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_GPIO_PIN,
      GPIO_Mode_AF,
      GPIO_Speed_2MHz,
      GPIO_OType_PP,
      GPIO_PuPd_NOPULL
  };

  TIM_TimeBaseInitTypeDef TIM_Base_InitStructure = {
      OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_PRELOAD,
      TIM_CounterMode_Up,
      OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_PERIOD,
      OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_CLOCK_DIV_0,
      OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_REPETITION_NO
  };

  TIM_OCInitTypeDef TIM_OC_InitStructure = {
      TIM_OCMode_PWM1,
      TIM_OutputState_Enable,
      TIM_OutputNState_Disable,
      OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_COMPARE_PULSE,
      TIM_OCPolarity_High,
      TIM_OCNPolarity_Low,
      TIM_OCIdleState_Reset,
      TIM_OCNIdleState_Reset
  };

  OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_CLK_ENABLE(OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_CLK,ENABLE);

  TIM_TimeBaseInit(OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER,&TIM_Base_InitStructure);
  OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_OC_CHANNEL_INIT(OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER,&TIM_OC_InitStructure);

  TIM_ARRPreloadConfig(OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER,ENABLE);
  TIM_OC2PreloadConfig(OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER,TIM_OCPreload_Enable);

  TIM_GenerateEvent(OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER,TIM_EventSource_COM);
  TIM_GenerateEvent(OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER,TIM_EventSource_Update);

  TIM_CCxCmd(OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER,OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_OC_CHANNEL,TIM_CCx_Enable);
  TIM_Cmd(OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER,ENABLE);

  RCC_AHB1PeriphClockCmd(OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_GPIO_CLK,ENABLE);

  GPIO_PinAFConfig(OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_GPIO_PORT,
                   OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_GPIO_PINSRC,
                   OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_GPIO_AF);

  GPIO_Init(OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_GPIO_PORT,&GPIO_InitStructure);
}

void OSC_Analog_Init(void){

  NVIC_InitTypeDef NVIC_InitStructure = {
      ADC_IRQn,
      OSC_ANALOG_ADC_PREEMP_PRIO,
      OSC_ANALOG_ADC_SUBPRIO,
      ENABLE
  }; /*AnalogWatchdog interrupt*/

  OSC_Analog_Init_DAC();
  OSC_Analog_Init_DMA();
  OSC_Analog_Init_ADC();
  OSC_Analog_Init_Timer();
  OSC_Analog_Init_1kHzSquareWave();

  ADC_Cmd(OSC_ANALOG_CHANNEL_A_ADC,ENABLE);   /*It should be here in order to avoid invalid conversion starts*/
  ADC_Cmd(OSC_ANALOG_CHANNEL_B_ADC,ENABLE);

  ADC_ITConfig(OSC_ANALOG_CHANNEL_A_ADC,ADC_IT_AWD,ENABLE);
  ADC_ITConfig(OSC_ANALOG_CHANNEL_B_ADC,ADC_IT_AWD,ENABLE);
  NVIC_Init(&NVIC_InitStructure);
}

OSC_Analog_Err_Type OSC_Analog_Conversion_Start(OSC_Analog_ChannelSelect_Type channelSelect){

  switch(channelSelect){        /*It doesn't stop the timer because it won't trigger any conversion if the ADC doesn't operate*/
    case OSC_Analog_ChannelSelect_Channel_A:
      OSC_ANALOG_CHANNEL_A_DMA_STREAM_FLAGS_CLEAR();
      DMA_Cmd(OSC_ANALOG_CHANNEL_A_DMA_STREAM,ENABLE);
      ADC_Cmd(OSC_ANALOG_CHANNEL_A_ADC, ENABLE);
      break;

    case OSC_Analog_ChannelSelect_Channel_B:
      OSC_ANALOG_CHANNEL_B_DMA_STREAM_FLAGS_CLEAR();
      DMA_Cmd(OSC_ANALOG_CHANNEL_B_DMA_STREAM,ENABLE);
      ADC_Cmd(OSC_ANALOG_CHANNEL_B_ADC, ENABLE);
      break;

    case OSC_Analog_ChannelSelect_ChannelBoth:    /*In this case the DMA address and length is ignored*/
      OSC_ANALOG_CHANNEL_A_DMA_STREAM_FLAGS_CLEAR();
      OSC_ANALOG_CHANNEL_B_DMA_STREAM_FLAGS_CLEAR();
      DMA_Cmd(OSC_ANALOG_CHANNEL_A_DMA_STREAM,ENABLE);
      DMA_Cmd(OSC_ANALOG_CHANNEL_B_DMA_STREAM,ENABLE);
      ADC_Cmd(OSC_ANALOG_CHANNEL_A_ADC, ENABLE);
      ADC_Cmd(OSC_ANALOG_CHANNEL_B_ADC, ENABLE);
      break;
    default:
      return OSC_Analog_Err_InvalidParameter;
  }
  return OSC_Analog_Err_OK;
}

OSC_Analog_Err_Type OSC_Analog_Conversion_Stop(OSC_Analog_ChannelSelect_Type   channelSelect){

  switch(channelSelect){        /*It doesn't stop the timer because it won't trigger any conversion if the ADC doesn't operate*/

    case OSC_Analog_ChannelSelect_Channel_A:
      ADC_Cmd(OSC_ANALOG_CHANNEL_A_ADC, DISABLE);
      ADC_ClearFlag(OSC_ANALOG_CHANNEL_A_ADC,OSC_ANALOG_CHANNEL_A_ADC_FLAGS);
      DMA_Cmd(OSC_ANALOG_CHANNEL_A_DMA_STREAM,DISABLE);
      break;

    case OSC_Analog_ChannelSelect_Channel_B:
      ADC_Cmd(OSC_ANALOG_CHANNEL_B_ADC, DISABLE);
      ADC_ClearFlag(OSC_ANALOG_CHANNEL_B_ADC, OSC_ANALOG_CHANNEL_B_ADC_FLAGS);
      DMA_Cmd(OSC_ANALOG_CHANNEL_B_DMA_STREAM,DISABLE);
      break;

    case OSC_Analog_ChannelSelect_ChannelBoth:
      ADC_Cmd(OSC_ANALOG_CHANNEL_A_ADC, DISABLE);
      ADC_Cmd(OSC_ANALOG_CHANNEL_B_ADC, DISABLE);
      ADC_ClearFlag(OSC_ANALOG_CHANNEL_A_ADC, OSC_ANALOG_CHANNEL_A_ADC_FLAGS);
      ADC_ClearFlag(OSC_ANALOG_CHANNEL_B_ADC, OSC_ANALOG_CHANNEL_B_ADC_FLAGS);

      DMA_Cmd(OSC_ANALOG_CHANNEL_A_DMA_STREAM,DISABLE);   /*DMA FIFO is not flushed immediately - it might take a little time*/
      DMA_Cmd(OSC_ANALOG_CHANNEL_B_DMA_STREAM,DISABLE);
      break;

    default:
      return OSC_Analog_Err_InvalidParameter;
  }

  return OSC_Analog_Err_OK;
}

OSC_Analog_Err_Type OSC_Analog_DMA_ReConfigure(
    OSC_Analog_ChannelSelect_Type   channelSelect,
    uint32_t*                       dataAcquisitionMemory,
    uint32_t                        datalength,
    OSC_Analog_DMA_Mode_Type        dmaMode
){
  switch(channelSelect){
    case OSC_Analog_ChannelSelect_Channel_A:
      if(dmaMode == OSC_Analog_DMA_Mode_Normal){
        OSC_ANALOG_CHANNEL_A_DMA_STREAM_MODE_NORMAL_SET();
      } else if(OSC_Analog_DMA_Mode_Circular){
        OSC_ANALOG_CHANNEL_A_DMA_STREAM_MODE_CIRCULAR_SET();
      } else {
        return OSC_Analog_Err_InvalidParameter;
      }
      DMA_SetCurrDataCounter(OSC_ANALOG_CHANNEL_A_DMA_STREAM,datalength);
      OSC_ANALOG_CHANNEL_A_DMA_STREAM_MEMORY_DEST_SET(dataAcquisitionMemory);
      break;

    case OSC_Analog_ChannelSelect_Channel_B:
      if(dmaMode == OSC_Analog_DMA_Mode_Normal){
        OSC_ANALOG_CHANNEL_B_DMA_STREAM_MODE_NORMAL_SET();
      } else if(OSC_Analog_DMA_Mode_Circular){
        OSC_ANALOG_CHANNEL_B_DMA_STREAM_MODE_CIRCULAR_SET();
      } else {
        return OSC_Analog_Err_InvalidParameter;
      }
      DMA_SetCurrDataCounter(OSC_ANALOG_CHANNEL_B_DMA_STREAM,datalength);
      OSC_ANALOG_CHANNEL_B_DMA_STREAM_MEMORY_DEST_SET(dataAcquisitionMemory);
      break;

    default:
      return OSC_Analog_Err_InvalidParameter;
  }
  return OSC_Analog_Err_OK;
}

OSC_Analog_Err_Type OSC_Analog_DMA_ReConfigureBothChannelOnTheFly(
    const OSC_Analog_Channel_DataAcquisitionConfig_Type* const channel_A_DMA_Configuration,
    const OSC_Analog_Channel_DataAcquisitionConfig_Type* const channel_B_DMA_Configuration
){
  uint32_t timer = OSC_ANALOG_DMA_DISABLE_TIMEOUT;

  OSC_ANALOG_CHANNEL_A_DMA_STREAM_STOP();
  OSC_ANALOG_CHANNEL_B_DMA_STREAM_STOP();

  /*Because of the FIFO flush the effective disable happens later*/
  while((OSC_ANALOG_CHANNEL_A_DMA_STREAM->CR & DMA_SxCR_EN) && (OSC_ANALOG_CHANNEL_B_DMA_STREAM->CR & DMA_SxCR_EN)){
    if(timer-- <= 0){
      return OSC_Analog_Err_Timeout;
    }
  }

  if(channel_A_DMA_Configuration->dmaMode == OSC_Analog_DMA_Mode_Normal){
    OSC_ANALOG_CHANNEL_A_DMA_STREAM_MODE_NORMAL_SET();
  } else if(OSC_Analog_DMA_Mode_Circular){
    OSC_ANALOG_CHANNEL_A_DMA_STREAM_MODE_CIRCULAR_SET();
  } else {
    return OSC_Analog_Err_InvalidParameter;
  }
  if(channel_B_DMA_Configuration->dmaMode == OSC_Analog_DMA_Mode_Normal){
    OSC_ANALOG_CHANNEL_B_DMA_STREAM_MODE_NORMAL_SET();
  } else if(OSC_Analog_DMA_Mode_Circular){
    OSC_ANALOG_CHANNEL_B_DMA_STREAM_MODE_CIRCULAR_SET();
  } else {
    return OSC_Analog_Err_InvalidParameter;
  }
  OSC_ANALOG_CHANNEL_A_DMA_STREAM_DATA_LENGTH_SET(channel_A_DMA_Configuration->datalength);
  OSC_ANALOG_CHANNEL_B_DMA_STREAM_DATA_LENGTH_SET(channel_B_DMA_Configuration->datalength);
  OSC_ANALOG_CHANNEL_A_DMA_STREAM_MEMORY_DEST_SET(channel_A_DMA_Configuration->dataAcquisitionMemory);
  OSC_ANALOG_CHANNEL_B_DMA_STREAM_MEMORY_DEST_SET(channel_B_DMA_Configuration->dataAcquisitionMemory);

  OSC_ANALOG_CHANNEL_A_DMA_STREAM_FLAGS_CLEAR();
  OSC_ANALOG_CHANNEL_B_DMA_STREAM_FLAGS_CLEAR();

  OSC_ANALOG_CHANNEL_A_DMA_STREAM_START();
  OSC_ANALOG_CHANNEL_B_DMA_STREAM_START();

  return OSC_Analog_Err_OK;
}

OSC_Analog_Err_Type OSC_Analog_AnalogWatchdog_Enable(ADC_TypeDef* triggerSourceADC, uint32_t middleThreshold, OSC_Analog_AnalogWatchdog_Range_Type range)
{
  if(range == OSC_Analog_AnalogWatchdog_Range_Lower){
    ADC_AnalogWatchdogThresholdsConfig(triggerSourceADC,middleThreshold & OSC_ANALOG_ANALOGWATCHDOG_MASK,0);
  } else if(range == OSC_Analog_AnalogWatchdog_Range_Upper){
    ADC_AnalogWatchdogThresholdsConfig(triggerSourceADC,0xFFF,middleThreshold & OSC_ANALOG_ANALOGWATCHDOG_MASK);
  } else {
    return OSC_Analog_Err_InvalidParameter;
  }
  ADC_AnalogWatchdogCmd(triggerSourceADC,ADC_AnalogWatchdog_SingleRegEnable);
  return OSC_Analog_Err_OK;
}

OSC_Analog_Err_Type OSC_Analog_AnalogWatchdog_Disable(void)
{
  ADC_AnalogWatchdogCmd(OSC_ANALOG_CHANNEL_A_ADC,ADC_AnalogWatchdog_None);
  ADC_AnalogWatchdogCmd(OSC_ANALOG_CHANNEL_B_ADC,ADC_AnalogWatchdog_None);
  return OSC_Analog_Err_OK;
}


/*void ADC_IRQHandler(void){
  static volatile uint8_t i = 0;

  STM32F4_Discovery_LEDToggle(LED6);
  if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC))
    i = ADC1->DR;
  if(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC))
    i = ADC2->DR;
}*/
