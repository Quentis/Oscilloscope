#include "OscilloscopeLED.h"

void OSC_LED_Init(void){
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  RCC_AHB1PeriphClockCmd(OSC_LED_CHANNEL_A_GPIO_CLK   | OSC_LED_CHANNEL_B_GPIO_CLK  |
                         OSC_LED_RANGE_MINUS_GPIO_CLK | OSC_LED_RANGE_PLUS_GPIO_CLK |
                         OSC_LED_TRIGGER_GPIO_CLK,ENABLE);

  GPIO_InitStructure.GPIO_Pin = OSC_LED_CHANNEL_A_GPIO_PIN;
  GPIO_Init(OSC_LED_CHANNEL_A_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_LED_CHANNEL_B_GPIO_PIN;
  GPIO_Init(OSC_LED_CHANNEL_B_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_LED_RANGE_MINUS_GPIO_PIN;
  GPIO_Init(OSC_LED_RANGE_MINUS_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_LED_RANGE_PLUS_GPIO_PIN;
  GPIO_Init(OSC_LED_RANGE_PLUS_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_LED_TRIGGER_GPIO_PIN;
  GPIO_Init(OSC_LED_TRIGGER_GPIO_PORT,&GPIO_InitStructure);
}
