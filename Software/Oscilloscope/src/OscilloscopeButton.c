#include "OscilloscopeButton.h"

static volatile OSC_Button_Value_Type OSC_Button_Value = 0;

void OSC_Button_Init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  OSC_BUTTON_SPI_CLK_INIT(OSC_BUTTON_SPI_CLK,ENABLE);
  RCC_AHB1PeriphClockCmd(OSC_BUTTON_SPI_SCK_GPIO_CLK  |  OSC_BUTTON_SPI_MISO_GPIO_CLK |
                         OSC_BUTTON_LOAD_GPIO_CLK     |  OSC_BUTTON_CLEAR_GPIO_CLK    |
                         OSC_BUTTON_SAMPLE_GPIO_CLK,ENABLE);

  GPIO_PinAFConfig(OSC_BUTTON_SPI_SCK_GPIO_PORT,OSC_BUTTON_SPI_SCK_SOURCE,OSC_BUTTON_SPI_SCK_AF);
  GPIO_PinAFConfig(OSC_BUTTON_SPI_MISO_GPIO_PORT,OSC_BUTTON_SPI_MISO_SOURCE,OSC_BUTTON_SPI_MISO_AF);

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_InitStructure.GPIO_Pin = OSC_BUTTON_SPI_SCK_PIN;
  GPIO_Init(OSC_BUTTON_SPI_SCK_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_BUTTON_SPI_MISO_PIN;
  GPIO_Init(OSC_BUTTON_SPI_MISO_GPIO_PORT,&GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

  GPIO_InitStructure.GPIO_Pin = OSC_BUTTON_LOAD_PIN;
  GPIO_Init(OSC_BUTTON_LOAD_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_BUTTON_CLEAR_PIN;
  GPIO_Init(OSC_BUTTON_CLEAR_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_BUTTON_SAMPLE_PIN;
  GPIO_Init(OSC_BUTTON_SAMPLE_GPIO_PORT,&GPIO_InitStructure);

  SPI_DeInit(OSC_BUTTON_SPI);

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL      = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
  SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;    /*42MHz / 256 = ~164kHz*/

  SPI_Init(OSC_BUTTON_SPI, &SPI_InitStructure);

  SPI_I2S_ITConfig(OSC_BUTTON_SPI,SPI_I2S_IT_RXNE,ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel                   = OSC_BUTTON_SPI_IRQ_CHANNEL;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = OSC_BUTTON_SPI_PREEMP_PRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = OSC_BUTTON_SPI_SUBPRIO;

  NVIC_Init(&NVIC_InitStructure);

  GPIO_SetBits(OSC_BUTTON_CLEAR_GPIO_PORT,OSC_BUTTON_CLEAR_PIN);

  SPI_Cmd(OSC_BUTTON_SPI,ENABLE);
}

void OSC_Button_Read(void){
  uint32_t i;
  GPIO_ResetBits(OSC_BUTTON_SAMPLE_GPIO_PORT,OSC_BUTTON_SAMPLE_PIN);
  for (i = 0; i < 10; ++i);
  GPIO_SetBits(OSC_BUTTON_SAMPLE_GPIO_PORT,OSC_BUTTON_SAMPLE_PIN);
  for (i = 0; i < 10; ++i);
  GPIO_ResetBits(OSC_BUTTON_LOAD_GPIO_PORT,OSC_BUTTON_LOAD_PIN);
  for (i = 0; i < 10; ++i);
  GPIO_SetBits(OSC_BUTTON_LOAD_GPIO_PORT,OSC_BUTTON_LOAD_PIN);
  for (i = 0; i < 10; ++i);
  SPI_I2S_SendData(OSC_BUTTON_SPI,OSC_BUTTON_READ_ONLY);
}

OSC_View_Event_Type OSC_Button_GetViewEventName(uint32_t buttonIndex,OSC_Button_EventQualifier_Type eventQualifier){
  switch(buttonIndex){
    case OSC_BUTTON_HORIZONTAL_LEFT:
      if(eventQualifier == OSC_Button_EventQualifier_Single)      return OSC_View_Event_HorizontalLeftClickSingle;
      else if(eventQualifier == OSC_Button_EventQualifier_Double) return OSC_View_Event_HorizontalLeftClickDouble;
      break;
    case OSC_BUTTON_HORIZONTAL_MIDDLE:
      if(eventQualifier == OSC_Button_EventQualifier_Single)      return OSC_View_Event_HorizontalMiddleClickSingle;
      else if(eventQualifier == OSC_Button_EventQualifier_Double) return OSC_View_Event_HorizontalMiddleClickDouble;
      break;
    case OSC_BUTTON_HORIZONTAL_RIGHT:
      if(eventQualifier == OSC_Button_EventQualifier_Single)      return OSC_View_Event_HorizontalRightClickSingle;
      else if(eventQualifier == OSC_Button_EventQualifier_Double) return OSC_View_Event_HorizontalRightClickDouble;
      break;
    case OSC_BUTTON_VERTICAL_UP:
      if(eventQualifier == OSC_Button_EventQualifier_Single)      return OSC_View_Event_VerticalUpClickSingle;
      else if(eventQualifier == OSC_Button_EventQualifier_Double) return OSC_View_Event_VerticalUpClickDouble;
      break;
    case OSC_BUTTON_VERTICAL_MIDDLE:
      if(eventQualifier == OSC_Button_EventQualifier_Single)      return OSC_View_Event_VerticalMiddleClickSingle;
      else if(eventQualifier == OSC_Button_EventQualifier_Double) return OSC_View_Event_VerticalMiddleClickDouble;
      break;
    case OSC_BUTTON_VERTICAL_DOWN:
      if(eventQualifier == OSC_Button_EventQualifier_Single)      return OSC_View_Event_VerticalDownClickSingle;
      else if(eventQualifier == OSC_Button_EventQualifier_Double) return OSC_View_Event_VerticalDownClickDouble;
      break;
    case OSC_BUTTON_TRIGGER_UP:
      if(eventQualifier == OSC_Button_EventQualifier_Single)      return OSC_View_Event_TriggerUpClickSingle;
      else if(eventQualifier == OSC_Button_EventQualifier_Double) return OSC_View_Event_TriggerUpClickDouble;
      break;
    case OSC_BUTTON_TRIGGER_DOWN:
      if(eventQualifier == OSC_Button_EventQualifier_Single)      return OSC_View_Event_TriggerDownClickSingle;
      else if(eventQualifier == OSC_Button_EventQualifier_Double) return OSC_View_Event_TriggerDownClickDouble;
      break;
  }
  return OSC_View_Event_EmptyEvent;
}

void OSC_Button_StateMachine(void){
  static OSC_Button_State_Type buttonState[OSC_BUTTON_COUNT] = {
      OSC_Button_State_Idle, OSC_Button_State_Idle, OSC_Button_State_Idle, OSC_Button_State_Idle,
      OSC_Button_State_Idle, OSC_Button_State_Idle, OSC_Button_State_Idle, OSC_Button_State_Idle
  };
  static OSC_Button_Counter_Type counterContinuousSingle   = 0;
  static OSC_Button_Counter_Type counterContinuousMultiple = 0;
  static OSC_Button_Counter_Type counterClickSingleTimeout = 0;
  static uint32_t i = 0;

  uint8_t buttonLogicLevel = OSC_Button_Value & (1 << i);
  OSC_View_Event_Type event;

  switch(buttonState[i]){
    case OSC_Button_State_Idle:
      if(buttonLogicLevel){
        buttonState[i] = OSC_Button_State_FirstRsgEdge_LevelHigh;
      } /* else: Nothing to do -> Stay in Idle*/
      break;
    case OSC_Button_State_FirstRsgEdge_LevelHigh:
      if(buttonLogicLevel){
        if(counterContinuousSingle == OSC_BUTTON_COUNTER_RADIX) {
          counterContinuousSingle = 0;
          buttonState[i] = OSC_Button_State_ContinuousSingle;
        } /*else: Nothing to do -> Stay in this state*/
        counterContinuousSingle++;
      } else {
        counterContinuousSingle = 0;
        buttonState[i] = OSC_Button_State_FirstFallingEdge_LevelLow;
      }
      break;
    case OSC_Button_State_ContinuousSingle:
      if(buttonLogicLevel){
        event = OSC_Button_GetViewEventName(i,OSC_Button_EventQualifier_Single);
        OSC_View_Event_RegisterEvent(event);
      } else {
        buttonState[i] = OSC_Button_State_Idle;
      }
      break;
    case OSC_Button_State_FirstFallingEdge_LevelLow:
      if(buttonLogicLevel){
        counterClickSingleTimeout = 0;
        buttonState[i] = OSC_Button_State_SecondRsgEdge_LevelHigh;
      } else {
        if(counterClickSingleTimeout == OSC_BUTTON_COUNTER_RADIX) {
          counterClickSingleTimeout = 0;
          event = OSC_Button_GetViewEventName(i,OSC_Button_EventQualifier_Single);
          OSC_View_Event_RegisterEvent(event);
          buttonState[i] = OSC_Button_State_Idle;
        }
        counterClickSingleTimeout++;
      }
      break;
    case OSC_Button_State_SecondRsgEdge_LevelHigh:
      if(buttonLogicLevel){
        if(counterContinuousMultiple == OSC_BUTTON_COUNTER_RADIX) {
          counterContinuousMultiple = 0;
          buttonState[i] = OSC_Button_State_ContinuousMultiple;
        } /*else: Nothing to do -> Stay in this state*/
        counterContinuousMultiple++;
      } else {
        counterContinuousMultiple = 0;
        event = OSC_Button_GetViewEventName(i,OSC_Button_EventQualifier_Double);
        OSC_View_Event_RegisterEvent(event);
        buttonState[i] = OSC_Button_State_Idle;
      }
      break;
    case OSC_Button_State_ContinuousMultiple:
      if(buttonLogicLevel){
        event = OSC_Button_GetViewEventName(i,OSC_Button_EventQualifier_Double);
        OSC_View_Event_RegisterEvent(event);
      } else {
        buttonState[i] = OSC_Button_State_Idle;
      }
      break;
  }
  i = (i + 1 != OSC_BUTTON_COUNT)? i + 1 : 0;
}

void SPI2_IRQHandler(void){
  uint8_t status      = SPI_I2S_GetITStatus(SPI2,SPI_I2S_IT_RXNE);
  if(status){
    OSC_Button_Value = ((~SPI_I2S_ReceiveData(SPI2)) & 0xFF);   /*The read operation clears the RXNE flag*/
  }                                                             /*Bit operations for active high logic*/
}
