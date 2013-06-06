#ifndef OSCILLOSCOPEBUTTON_H_
#define OSCILLOSCOPEBUTTON_H_

#include "stm32f4xx_conf.h"
#include "OscilloscopeView.h"

#define OSC_BUTTON_SPI                           SPI2
#define OSC_BUTTON_SPI_CLK                       RCC_APB1Periph_SPI2
#define OSC_BUTTON_SPI_CLK_INIT                  RCC_APB1PeriphClockCmd
#define OSC_BUTTON_SPI_IRQ_CHANNEL               SPI2_IRQn
#define OSC_BUTTON_SPI_PREEMP_PRIO               0x1
#define OSC_BUTTON_SPI_SUBPRIO                   0x7

#define OSC_BUTTON_SPI_SCK_PIN                   GPIO_Pin_13
#define OSC_BUTTON_SPI_SCK_GPIO_PORT             GPIOB
#define OSC_BUTTON_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define OSC_BUTTON_SPI_SCK_SOURCE                GPIO_PinSource13
#define OSC_BUTTON_SPI_SCK_AF                    GPIO_AF_SPI2

#define OSC_BUTTON_SPI_MISO_PIN                  GPIO_Pin_14
#define OSC_BUTTON_SPI_MISO_GPIO_PORT            GPIOB
#define OSC_BUTTON_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define OSC_BUTTON_SPI_MISO_SOURCE               GPIO_PinSource14
#define OSC_BUTTON_SPI_MISO_AF                   GPIO_AF_SPI2

#define OSC_BUTTON_LOAD_PIN                      GPIO_Pin_12
#define OSC_BUTTON_LOAD_GPIO_PORT                GPIOB
#define OSC_BUTTON_LOAD_GPIO_CLK                 RCC_AHB1Periph_GPIOB

#define OSC_BUTTON_CLEAR_PIN                     GPIO_Pin_11
#define OSC_BUTTON_CLEAR_GPIO_PORT               GPIOB
#define OSC_BUTTON_CLEAR_GPIO_CLK                RCC_AHB1Periph_GPIOB

#define OSC_BUTTON_SAMPLE_PIN                    GPIO_Pin_15
#define OSC_BUTTON_SAMPLE_GPIO_PORT              GPIOB
#define OSC_BUTTON_SAMPLE_GPIO_CLK               RCC_AHB1Periph_GPIOB

#define OSC_BUTTON_READ_ONLY   0

#define OSC_BUTTON_COUNTER_RADIX                 5
#define OSC_BUTTON_COUNT                         8

#define OSC_BUTTON_HORIZONTAL_LEFT               5
#define OSC_BUTTON_HORIZONTAL_MIDDLE             6
#define OSC_BUTTON_HORIZONTAL_RIGHT              7
#define OSC_BUTTON_VERTICAL_UP                   2
#define OSC_BUTTON_VERTICAL_MIDDLE               1
#define OSC_BUTTON_VERTICAL_DOWN                 0
#define OSC_BUTTON_TRIGGER_UP                    4
#define OSC_BUTTON_TRIGGER_DOWN                  3

typedef enum {
  OSC_Button_State_Idle,
  OSC_Button_State_FirstRsgEdge_LevelHigh,
  OSC_Button_State_ContinuousSingle,
  OSC_Button_State_FirstFallingEdge_LevelLow,
  OSC_Button_State_SecondRsgEdge_LevelHigh,
  OSC_Button_State_ContinuousMultiple
} OSC_Button_State_Type;

typedef enum {
  OSC_Button_EventQualifier_Single,
  OSC_Button_EventQualifier_Double
} OSC_Button_EventQualifier_Type;

typedef uint8_t OSC_Button_Value_Type;
typedef uint8_t OSC_Button_Counter_Type;

void OSC_Button_Init(void);
void OSC_Button_Read(void);
void OSC_Button_StateMachine(void);
OSC_View_Event_Type OSC_Button_GetViewEventName(uint32_t buttonIndex,OSC_Button_EventQualifier_Type eventQualifier);

#endif /* OSCILLOSCOPEBUTTON_H_ */
