#ifndef OSCILLOSCOPEANALOG_H_
#define OSCILLOSCOPEANALOG_H_

#include "stm32f4xx_conf.h"
#include "stm32f4xx_dma.h"
#include "OscilloscopeConfiguration.h"

typedef enum {
  OSC_Analog_DMA_Mode_Normal,
  OSC_Analog_DMA_Mode_Circular
} OSC_Analog_DMA_Mode_Type;

typedef enum {
  OSC_Analog_ChannelSelect_Channel_A,
  OSC_Analog_ChannelSelect_Channel_B,
  OSC_Analog_ChannelSelect_ChannelBoth
} OSC_Analog_ChannelSelect_Type;

typedef enum {
  OSC_Analog_Err_OK,
  OSC_Analog_Err_ReStart,
  OSC_Analog_Err_ReStop,
  OSC_Analog_Err_InvalidParameter,
  OSC_Analog_Err_Consistency,
  OSC_Analog_Err_Timeout
} OSC_Analog_Err_Type;

typedef enum {
  OSC_Analog_TriggerStatus_DataOppositePolarity,
  OSC_Analog_TriggerStatus_DataProperPolarity,
  OSC_Analog_TriggerStatus_TriggerEvent
} OSC_Analog_TriggerStatus_Type;

typedef struct {
  uint32_t*                       dataAcquisitionMemory;
  uint32_t                        datalength;
  OSC_Analog_DMA_Mode_Type        dmaMode;
} OSC_Analog_Channel_DataAcquisitionConfig_Type;


/*=======================================ADC_DEFINITIONS=======================================*/
#define OSC_ANALOG_CHANNEL_A_ADC                    ADC1
#define OSC_ANALOG_CHANNEL_B_ADC                    ADC2

#define OSC_ANALOG_CHANNEL_A_ADC_CHANNEL            ADC_Channel_1
#define OSC_ANALOG_CHANNEL_B_ADC_CHANNEL            ADC_Channel_2

#define OSC_ANALOG_CHANNEL_A_ADC_FLAGS              (ADC_FLAG_AWD   | ADC_FLAG_EOC | ADC_FLAG_JEOC |\
                                                     ADC_FLAG_JSTRT | ADC_FLAG_OVR | ADC_FLAG_STRT)
#define OSC_ANALOG_CHANNEL_B_ADC_FLAGS              (ADC_FLAG_AWD   | ADC_FLAG_EOC | ADC_FLAG_JEOC |\
                                                     ADC_FLAG_JSTRT | ADC_FLAG_OVR | ADC_FLAG_STRT)

#define OSC_ANALOG_CHANNEL_A_ADC_SEQRANK            1
#define OSC_ANALOG_CHANNEL_B_ADC_SEQRANK            1

#define OSC_ANALOG_ADC_PREEMP_PRIO                  0x0
#define OSC_ANALOG_ADC_SUBPRIO                      0x0

#define OSC_ANALOG_ADC_SCAN_DISABLED                DISABLE
#define OSC_ANALOG_ADC_CONTINUOUSMODE_DISABLED      DISABLE
#define OSC_ANALOG_ADC_NUM_OF_CONV                  0
#define OSC_ANALOG_ADC_PRESCALER_FOR_21MHZ          ADC_Prescaler_Div4
#define OSC_ANALOG_ADC_CLK_ENABLE                   RCC_APB2PeriphClockCmd

#define OSC_ANALOG_CHANNEL_A_ADC_CLK                RCC_APB2Periph_ADC1
#define OSC_ANALOG_CHANNEL_A_ADC_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define OSC_ANALOG_CHANNEL_A_ADC_GPIO_PORT          GPIOA
#define OSC_ANALOG_CHANNEL_A_ADC_GPIO_PIN           GPIO_Pin_1

#define OSC_ANALOG_CHANNEL_B_ADC_CLK                RCC_APB2Periph_ADC2
#define OSC_ANALOG_CHANNEL_B_ADC_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define OSC_ANALOG_CHANNEL_B_ADC_GPIO_PORT          GPIOA
#define OSC_ANALOG_CHANNEL_B_ADC_GPIO_PIN           GPIO_Pin_2

/*=======================================DMA_DEFINITIONS=======================================*/
/*DMA_CHANNEL_COMMON_DEFINITIONS*/
#define OSC_ANALOG_DMA_DISABLE_TIMEOUT    20

/*DMA_CHANNEL_A_DEFINITIONS*/
#define OSC_ANALOG_CHANNEL_A_DMA_CLK                        RCC_AHB1Periph_DMA2
#define OSC_ANALOG_CHANNEL_A_DMA_STREAM                     DMA2_Stream0   /*It is important: Channel A is ADC1*/

#define OSC_ANALOG_CHANNEL_A_DMA_FLAGS                      (DMA_FLAG_DMEIF0 | DMA_FLAG_FEIF0 | DMA_FLAG_HTIF0 |\
                                                             DMA_FLAG_TCIF0  | DMA_FLAG_TEIF0)

#define OSC_ANALOG_CHANNEL_A_DMA_STREAM_MODE_CIRCULAR_SET()        do{OSC_ANALOG_CHANNEL_A_DMA_STREAM->CR   |=  DMA_Mode_Circular;         }while(0)
#define OSC_ANALOG_CHANNEL_A_DMA_STREAM_MODE_NORMAL_SET()          do{OSC_ANALOG_CHANNEL_A_DMA_STREAM->CR   &= ~DMA_Mode_Circular;         }while(0)
#define OSC_ANALOG_CHANNEL_A_DMA_STREAM_MEMORY_DEST_SET(dest)      do{OSC_ANALOG_CHANNEL_A_DMA_STREAM->M0AR =  ((uint32_t)dest);           }while(0)
#define OSC_ANALOG_CHANNEL_A_DMA_STREAM_DATA_LENGTH_SET(len)       do{OSC_ANALOG_CHANNEL_A_DMA_STREAM->NDTR =  ((uint32_t)(len) & 0xFFFF); }while(0)

#define OSC_ANALOG_CHANNEL_A_DMA_CHANNEL                           DMA_Channel_0
#define OSC_ANALOG_CHANNEL_A_DMA_PERIPH_BASE_ADDRESS               ((uint32_t)(&OSC_ANALOG_CHANNEL_A_ADC->DR))
#define OSC_ANALOG_CHANNEL_A_DMA_MEMORY0_BASE_ADDRESS              0    /*It must be set at the start of data acquisition anyway*/
#define OSC_ANALOG_CHANNEL_A_DMA_DIR                               DMA_DIR_PeripheralToMemory
#define OSC_ANALOG_CHANNEL_A_DMA_BUFFER_SIZE                       0    /*It must be set at the start of data acquisition anyway*/
#define OSC_ANALOG_CHANNEL_A_DMA_PERIPH_ADDRESS_INC                DMA_PeripheralInc_Disable
#define OSC_ANALOG_CHANNEL_A_DMA_MEMORY_ADDRESS_INC                DMA_MemoryInc_Enable
#define OSC_ANALOG_CHANNEL_A_DMA_PERIPH_DATA_SIZE                  DMA_PeripheralDataSize_Byte
#define OSC_ANALOG_CHANNEL_A_DMA_MEMORY_DATA_SIZE                  DMA_MemoryDataSize_Word
#define OSC_ANALOG_CHANNEL_A_DMA_MODE                              DMA_Mode_Circular
#define OSC_ANALOG_CHANNEL_A_DMA_PRIO                              DMA_Priority_High
#define OSC_ANALOG_CHANNEL_A_DMA_FIFO_MODE                         DMA_FIFOMode_Enable
#define OSC_ANALOG_CHANNEL_A_DMA_FIFO_THRESHOLD                    DMA_FIFOThreshold_Full
#define OSC_ANALOG_CHANNEL_A_DMA_MEMORY_BURST                      DMA_MemoryBurst_Single
#define OSC_ANALOG_CHANNEL_A_DMA_PERIPH_BURST                      DMA_PeripheralBurst_Single



/*DMA_CHANNEL_B_DEFINITIONS*/
#define OSC_ANALOG_CHANNEL_B_DMA_CLK                               RCC_AHB1Periph_DMA2
#define OSC_ANALOG_CHANNEL_B_DMA_STREAM                            DMA2_Stream2   /*It is important: Channel B is ADC2*/

#define OSC_ANALOG_CHANNEL_B_DMA_FLAGS                             (DMA_FLAG_DMEIF2 | DMA_FLAG_FEIF2 | DMA_FLAG_HTIF2 |\
                                                                    DMA_FLAG_TCIF2  | DMA_FLAG_TEIF2)

#define OSC_ANALOG_CHANNEL_B_DMA_STREAM_MODE_CIRCULAR_SET()        do{OSC_ANALOG_CHANNEL_B_DMA_STREAM->CR |=  DMA_Mode_Circular;           }while(0)
#define OSC_ANALOG_CHANNEL_B_DMA_STREAM_MODE_NORMAL_SET()          do{OSC_ANALOG_CHANNEL_B_DMA_STREAM->CR &= ~DMA_Mode_Circular;           }while(0)
#define OSC_ANALOG_CHANNEL_B_DMA_STREAM_MEMORY_DEST_SET(dest)      do{OSC_ANALOG_CHANNEL_B_DMA_STREAM->M0AR = ((uint32_t)dest);            }while(0)
#define OSC_ANALOG_CHANNEL_B_DMA_STREAM_DATA_LENGTH_SET(len)       do{OSC_ANALOG_CHANNEL_B_DMA_STREAM->NDTR =  ((uint32_t)(len) & 0xFFFF); }while(0)

#define OSC_ANALOG_CHANNEL_B_DMA_CHANNEL                           DMA_Channel_1
#define OSC_ANALOG_CHANNEL_B_DMA_PERIPH_BASE_ADDRESS               ((uint32_t)(&OSC_ANALOG_CHANNEL_B_ADC->DR))
#define OSC_ANALOG_CHANNEL_B_DMA_MEMORY0_BASE_ADDRESS              0    /*It must be set at the start of data acquisition anyway*/
#define OSC_ANALOG_CHANNEL_B_DMA_DIR                               DMA_DIR_PeripheralToMemory
#define OSC_ANALOG_CHANNEL_B_DMA_BUFFER_SIZE                       0    /*It must be set at the start of data acquisition anyway*/
#define OSC_ANALOG_CHANNEL_B_DMA_PERIPH_ADDRESS_INC                DMA_PeripheralInc_Disable
#define OSC_ANALOG_CHANNEL_B_DMA_MEMORY_ADDRESS_INC                DMA_MemoryInc_Enable
#define OSC_ANALOG_CHANNEL_B_DMA_PERIPH_DATA_SIZE                  DMA_PeripheralDataSize_Byte
#define OSC_ANALOG_CHANNEL_B_DMA_MEMORY_DATA_SIZE                  DMA_MemoryDataSize_Word
#define OSC_ANALOG_CHANNEL_B_DMA_MODE                              DMA_Mode_Circular
#define OSC_ANALOG_CHANNEL_B_DMA_PRIO                              DMA_Priority_Medium
#define OSC_ANALOG_CHANNEL_B_DMA_FIFO_MODE                         DMA_FIFOMode_Enable
#define OSC_ANALOG_CHANNEL_B_DMA_FIFO_THRESHOLD                    DMA_FIFOThreshold_Full
#define OSC_ANALOG_CHANNEL_B_DMA_MEMORY_BURST                      DMA_MemoryBurst_Single
#define OSC_ANALOG_CHANNEL_B_DMA_PERIPH_BURST                      DMA_PeripheralBurst_Single

/*======================================TIMER_DEFINITIONS======================================*/
#define OSC_ANALOG_SAMPLE_TIMER                      TIM1
#define OSC_ANALOG_SAMPLE_TIMER_CLK                  RCC_APB2Periph_TIM1
#define OSC_ANALOG_SAMPLE_TIMER_CLK_ENABLE           RCC_APB2PeriphClockCmd     /*168 MHz base clock*/

#define OSC_ANALOG_SAMPLE_TIMER_PRELOAD              167     /*4MHz clock for the timer*/     //20
#define OSC_ANALOG_SAMPLE_TIMER_PERIOD               999     /*1MHz -> timer overflow*/   //7
#define OSC_ANALOG_SAMPLE_TIMER_CLOCK_DIV_0          0
#define OSC_ANALOG_SAMPLE_TIMER_REPETITION_NO        0

#define OSC_ANALOG_SAMPLE_TIMER_COMPARE_PULSE        500     /*1MHz for compare event*/    //3

#define OSC_ANALOG_SAMPLE_TIMER_DEADTIME             0     /*Dummy*/



#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER                     TIM3
#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_CLK                 RCC_APB1Periph_TIM3
#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_CLK_ENABLE          RCC_APB1PeriphClockCmd         /*84 MHz base clock*/

#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_OC_CHANNEL_INIT     TIM_OC4Init
#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_OC_CHANNEL          TIM_Channel_4

#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_PRELOAD             83       /*1MHz clock for the timer*/
#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_PERIOD              999      /*1kHz -> timer overflow*/
#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_CLOCK_DIV_0         0
#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_REPETITION_NO       0

#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_COMPARE_PULSE       500      /*1MHz for compare event*/

#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_GPIO_CLK            RCC_AHB1Periph_GPIOB
#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_GPIO_PORT           GPIOB
#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_GPIO_PIN            GPIO_Pin_1
#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_GPIO_AF             GPIO_AF_TIM3
#define OSC_ANALOG_1KHZ_SQUARE_WAVE_TIMER_GPIO_PINSRC         GPIO_PinSource1


/*=======================================DAC_DEFINITIONS=======================================*/

#define OSC_ANALOG_DAC_CLK_ENABLE               RCC_APB1PeriphClockCmd

#define OSC_ANALOG_DAC                          DAC
#define OSC_ANALOG_DAC_CHANNEL                  DAC_Channel_1
#define OSC_ANALOG_DAC_CLK                      RCC_APB1Periph_DAC
#define OSC_ANALOG_DAC_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define OSC_ANALOG_DAC_GPIO_PORT                GPIOA
#define OSC_ANALOG_DAC_GPIO_PIN                 GPIO_Pin_4

/*
 * The voltage on the output of the DAC should be 0.83333 V
 * The formula for the voltage based on the reference manual:
 *        Vout = Vref * (DOR / 4095)
 *        DOD  = 4095 * (Vout / Vref) = 1169 = 0x491
 *            Vref = 2.92 V based on measurement
 *            Vout = 0.83 V
 */

#define OSC_ANALOG_DAC_OUPUT_VALUE              0x491

void OSC_Analog_Init(void);
void OSC_Analog_Init_DAC(void);
void OSC_Analog_Init_ADC(void);
void OSC_Analog_Init_Timer(void);

void OSC_Analog_Init_1kHzSquareWave(void);
void OSC_Analog_Init_DMA(void);

#endif /* OSCILLOSCOPEANALOG_H_ */
