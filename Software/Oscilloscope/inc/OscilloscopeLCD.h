#ifndef OSCILLOSCOPELCD_H_
#define OSCILLOSCOPELCD_H_

#include "stm32f4xx_conf.h"
#include "stm32f4_discovery.h"
#include "dt64128q_LCD.h"

/*====================================== LOWLEVEL_DEFINITIONS ======================================*/

#define OSC_LCD_RW_GPIO_CLK       RCC_AHB1Periph_GPIOE
#define OSC_LCD_RW_GPIO_PORT      GPIOE
#define OSC_LCD_RW_GPIO_PIN       GPIO_Pin_7
#define OSC_LCD_RW_BIT_SET()      OSC_LCD_RW_GPIO_PORT->BSRRL = OSC_LCD_RW_GPIO_PIN
#define OSC_LCD_RW_BIT_CLEAR()    OSC_LCD_RW_GPIO_PORT->BSRRH = OSC_LCD_RW_GPIO_PIN

#define OSC_LCD_EN_GPIO_CLK       RCC_AHB1Periph_GPIOB
#define OSC_LCD_EN_GPIO_PORT      GPIOB
#define OSC_LCD_EN_GPIO_PIN       GPIO_Pin_7
#define OSC_LCD_EN_BIT_SET()      OSC_LCD_EN_GPIO_PORT->BSRRL = OSC_LCD_EN_GPIO_PIN
#define OSC_LCD_EN_BIT_CLEAR()    OSC_LCD_EN_GPIO_PORT->BSRRH = OSC_LCD_EN_GPIO_PIN

#define OSC_LCD_RST_GPIO_CLK      RCC_AHB1Periph_GPIOD
#define OSC_LCD_RST_GPIO_PORT     GPIOD
#define OSC_LCD_RST_GPIO_PIN      GPIO_Pin_3
#define OSC_LCD_RST_BIT_SET()     OSC_LCD_RST_GPIO_PORT->BSRRL = OSC_LCD_RST_GPIO_PIN
#define OSC_LCD_RST_BIT_CLEAR()   OSC_LCD_RST_GPIO_PORT->BSRRH = OSC_LCD_RST_GPIO_PIN

#define OSC_LCD_CS1_GPIO_CLK      RCC_AHB1Periph_GPIOB
#define OSC_LCD_CS1_GPIO_PORT     GPIOB
#define OSC_LCD_CS1_GPIO_PIN      GPIO_Pin_4
#define OSC_LCD_CS1_BIT_SET()     OSC_LCD_CS1_GPIO_PORT->BSRRL = OSC_LCD_CS1_GPIO_PIN
#define OSC_LCD_CS1_BIT_CLEAR()   OSC_LCD_CS1_GPIO_PORT->BSRRH = OSC_LCD_CS1_GPIO_PIN

#define OSC_LCD_CS2_GPIO_CLK      RCC_AHB1Periph_GPIOB
#define OSC_LCD_CS2_GPIO_PORT     GPIOB
#define OSC_LCD_CS2_GPIO_PIN      GPIO_Pin_5
#define OSC_LCD_CS2_BIT_SET()     OSC_LCD_CS2_GPIO_PORT->BSRRL = OSC_LCD_CS2_GPIO_PIN
#define OSC_LCD_CS2_BIT_CLEAR()   OSC_LCD_CS2_GPIO_PORT->BSRRH = OSC_LCD_CS2_GPIO_PIN

#define OSC_LCD_E_GPIO_CLK        RCC_AHB1Periph_GPIOD
#define OSC_LCD_E_GPIO_PORT       GPIOD
#define OSC_LCD_E_GPIO_PIN        GPIO_Pin_7
#define OSC_LCD_E_BIT_SET()       OSC_LCD_E_GPIO_PORT->BSRRL = OSC_LCD_E_GPIO_PIN
#define OSC_LCD_E_BIT_CLEAR()     OSC_LCD_E_GPIO_PORT->BSRRH = OSC_LCD_E_GPIO_PIN

#define OSC_LCD_DC_GPIO_CLK       RCC_AHB1Periph_GPIOD
#define OSC_LCD_DC_GPIO_PORT      GPIOD
#define OSC_LCD_DC_GPIO_PIN       GPIO_Pin_6
#define OSC_LCD_DC_BIT_SET()      OSC_LCD_DC_GPIO_PORT->BSRRL = OSC_LCD_DC_GPIO_PIN
#define OSC_LCD_DC_BIT_CLEAR()    OSC_LCD_DC_GPIO_PORT->BSRRH = OSC_LCD_DC_GPIO_PIN

#define OSC_LCD_BL_PWM_GPIO_CLK   RCC_AHB1Periph_GPIOC
#define OSC_LCD_BL_PWM_GPIO_PORT  GPIOC
#define OSC_LCD_BL_PWM_GPIO_PIN   GPIO_Pin_8
#define OSC_LCD_BL_BIT_SET()      OSC_LCD_BL_PWM_GPIO_PORT->BSRRL = OSC_LCD_BL_PWM_GPIO_PIN
#define OSC_LCD_BL_BIT_CLEAR()    OSC_LCD_BL_PWM_GPIO_PORT->BSRRH = OSC_LCD_BL_PWM_GPIO_PIN


#define OSC_LCD_DATA0_GPIO_PIN    GPIO_Pin_8
#define OSC_LCD_DATA1_GPIO_PIN    GPIO_Pin_9
#define OSC_LCD_DATA2_GPIO_PIN    GPIO_Pin_10
#define OSC_LCD_DATA3_GPIO_PIN    GPIO_Pin_11
#define OSC_LCD_DATA4_GPIO_PIN    GPIO_Pin_12
#define OSC_LCD_DATA5_GPIO_PIN    GPIO_Pin_13
#define OSC_LCD_DATA6_GPIO_PIN    GPIO_Pin_14
#define OSC_LCD_DATA7_GPIO_PIN    GPIO_Pin_15

#define OSC_LCD_DATABUS_GPIO_CLK    RCC_AHB1Periph_GPIOE
#define OSC_LCD_DATABUS_GPIO_PORT   GPIOE
#define OSC_LCD_DATABUS_GPIO_PINS   (OSC_LCD_DATA0_GPIO_PIN|OSC_LCD_DATA1_GPIO_PIN|OSC_LCD_DATA2_GPIO_PIN|\
                                    OSC_LCD_DATA3_GPIO_PIN|OSC_LCD_DATA4_GPIO_PIN|OSC_LCD_DATA5_GPIO_PIN|\
                                    OSC_LCD_DATA6_GPIO_PIN|OSC_LCD_DATA7_GPIO_PIN)


#define OSC_LCD_DATABUS_GPIO_PIN_OFFSET   8
#define OSC_LCD_DATABUS_GPIO_ODR          OSC_LCD_DATABUS_GPIO_PORT->ODR
#define OSC_LCD_DATABUS_GPIO_IDR          OSC_LCD_DATABUS_GPIO_PORT->IDR
#define OSC_LCD_DATABUS_WRITE(DATA)       OSC_LCD_DATABUS_GPIO_ODR = ((OSC_LCD_DATABUS_GPIO_ODR & (~OSC_LCD_DATABUS_GPIO_PINS)) |\
                                          (DATA << OSC_LCD_DATABUS_GPIO_PIN_OFFSET))
#define OSC_LCD_DATABUS_READ()            ((OSC_LCD_DATABUS_GPIO_IDR & OSC_LCD_DATABUS_GPIO_PINS) >> OSC_LCD_DATABUS_GPIO_PIN_OFFSET)

#define OSC_LCD_DATABUS_HW_DIR_REGISTER           OSC_LCD_DATABUS_GPIO_PORT->MODER
#define OSC_LCD_DATABUS_HW_DIR_REGISTER_MASK      0xFFFF0000
#define OSC_LCD_DATABUS_HW_DIR_REGISTER_READ      0x00000000
#define OSC_LCD_DATABUS_HW_DIR_REGISTER_WRITE     0x55550000


#define OSC_LCD_DATABUS_HW_DIR_SET_READ()     do{OSC_LCD_DATABUS_HW_DIR_REGISTER =\
                                               ((OSC_LCD_DATABUS_HW_DIR_REGISTER & ~OSC_LCD_DATABUS_HW_DIR_REGISTER_MASK) |\
                                                 OSC_LCD_DATABUS_HW_DIR_REGISTER_READ);\
                                                 OSC_LCD_RW_BIT_SET();\
                                              }while(0)

#define OSC_LCD_DATABUS_HW_DIR_SET_WRITE()    do{ OSC_LCD_RW_BIT_CLEAR();\
                                                  OSC_LCD_DATABUS_HW_DIR_REGISTER =\
                                                ((OSC_LCD_DATABUS_HW_DIR_REGISTER & ~OSC_LCD_DATABUS_HW_DIR_REGISTER_MASK) |\
                                                  OSC_LCD_DATABUS_HW_DIR_REGISTER_WRITE);\
                                              }while(0)

#define OSC_LCD_DATABUS_HW_DIR_GET()         (OSC_LCD_DATABUS_HW_DIR_REGISTER)

#define OSC_LCD_DATABUS_HW_DIR_WRITE         OSC_LCD_DATABUS_HW_DIR_REGISTER_WRITE
#define OSC_LCD_DATABUS_HW_DIR_READ          OSC_LCD_DATABUS_HW_DIR_REGISTER_READ

#define  OSC_LCD_CMD_ON_OFF               DT64128Q_LCD_CMD_ON_OFF
#define  OSC_LCD_CMD_ON                   DT64128Q_LCD_CMD_ON
#define  OSC_LCD_CMD_OFF                  DT64128Q_LCD_CMDOFF
#define  OSC_LCD_CMD_STARTLINE            DT64128Q_LCD_CMD_SET_START_LINE
#define  OSC_LCD_CMD_STARTLINE_MASK       DT64128Q_LCD_CMD_SET_START_LINE_MASK
#define  OSC_LCD_CMD_DATAPAGE             DT64128Q_LCD_CMD_SET_DATAPAGE_ADDRESS
#define  OSC_LCD_CMD_DATAPAGE_MASK        DT64128Q_LCD_CMD_SET_DATAPAGE_ADDRESS_MASK
#define  OSC_LCD_CMD_COLUMNADDRESS        DT64128Q_LCD_CMD_SET_COLUMN_ADDRESS
#define  OSC_LCD_CMD_COLUMNADDRESS_MASK   DT64128Q_LCD_CMD_SET_COLUMN_ADDRESS_MASK

#define   OSC_LCD_TIM               TIM6
#define   OSC_LCD_TIM_CLK           RCC_APB1Periph_TIM6
#define   OSC_LCD_TIM_IRQCHN        TIM6_DAC_IRQn
#define   OSC_LCD_TIM_PREEMP_PRIO   0x0
#define   OSC_LCD_TIM_SUBPRIO       0x2

#define   OSC_LCD_TIM_WAIT_1US()    TIM_Cmd(OSC_LCD_TIM,ENABLE)

/*
 * APB1 -> 42MHz and APB1 prescaler is /1 ==> TIM6_CLK = 42MHz
 * The waveform needs at least 1us resolution -> 42 should be the period
 */
#define   OSC_LCD_TIMER_PERIOD        42
#define   OSC_LCD_TIMER_ARR_VALUE     (OSC_LCD_TIMER_PERIOD - 1)

/*Backlight PWM definitions*/
#define OSC_LCD_BL_PWM_TIMER                        TIM8
#define OSC_LCD_BL_PWM_TIMER_CLK                    RCC_APB2Periph_TIM8
#define OSC_LCD_BL_PWM_TIMER_CLK_ENABLE             RCC_APB2PeriphClockCmd
#define OSC_LCD_BL_PWM_TIMER_DEADTIME               0   /*Dummy*/

/* APB2 timer -> 2 * APB2 clock frequency = 168MHz */
#define OSC_LCD_BL_PWM_TIMER_PRELOAD                1679  /*DIV 1680*/
#define OSC_LCD_BL_PWM_TIMER_PERIOD                   99  /*DIV 100*/
#define OSC_LCD_BL_PWM_TIMER_CLOCK_DIV_0               0
#define OSC_LCD_BL_PWM_TIMER_REPETITION_NO             0

#define OSC_LCD_BL_PWM_TIMER_OC_CHANNEL_INIT        TIM_OC3Init
#define OSC_LCD_BL_PWM_TIMER_OC_PRELOAD_CONFIG      TIM_OC3PreloadConfig
#define OSC_LCD_BL_PWM_TIMER_OC_COMPARE_SET         TIM_SetCompare3
#define OSC_LCD_BL_PWM_TIMER_OC_COMPARE_PULSE          99
#define OSC_LCD_BL_PWM_TIMER_OC_CHANNEL             TIM_Channel_3

#define OSC_LCD_BL_PWM_TIMER_GPIO_PORT              GPIOC
#define OSC_LCD_BL_PWM_TIMER_GPIO_PINSRC            GPIO_PinSource8
#define OSC_LCD_BL_PWM_TIMER_GPIO_AF                GPIO_AF_TIM8
#define OSC_LCD_BL_PWM_TIMER_GPIO_CLK               RCC_AHB1Periph_GPIOC


/*======================================== ERROR_DEFINITIONS =======================================*/

typedef enum{
  OSC_LCD_Err_OK,
  OSC_LCD_Err_NoChip_Selected,
  OSC_LCD_Err_DataBus_Busy,
  OSC_LCD_Err_BufferFull,
  OSC_LCD_Err_InvalidParameter
} OSC_LCD_Err_Type;

/*======================================= DRIVER_STATUS_TYPES ======================================*/

typedef enum {
  OSC_LCD_DriverTransfer_Status_InProgress,
  OSC_LCD_DriverTransfer_Status_Completed
} OSC_LCD_DriverTransfer_Status_Type;

typedef enum {
  OSC_LCD_DriverTransfer_PayloadType_DataPageAddress,
  OSC_LCD_DriverTransfer_PayloadType_ColumnAddress,
  OSC_LCD_DriverTransfer_PayloadType_Data
} OSC_LCD_DriverTransfer_PayloadType_Type;

typedef enum {
  OSC_LCD_DriverTransfer_InnerState_StartToTransmit,
  OSC_LCD_DriverTransfer_InnerState_PayloadSent,
  OSC_LCD_DriverTransfer_InnerState_StatusRead
} OSC_LCD_DriverTransfer_InnerState_Type;

typedef enum {
  OSC_LCD_DataBus_Status_Busy,
  OSC_LCD_DataBus_Status_Free
} OSC_LCD_DataBus_Status_Type;

typedef enum {
  OSC_LCD_DataBus_Direction_Read,
  OSC_LCD_DataBus_Direction_Write
} OSC_LCD_DataBus_Direction_Type;

typedef enum {
  OSC_LCD_DataBus_CmdData_Command,
  OSC_LCD_DataBus_CmdData_Data
} OSC_LCD_DataBus_CmdData_Type;

typedef enum {
  OSC_LCD_ChipSelect_NoChip_Selected,
  OSC_LCD_ChipSelect_Chip1_Selected,
  OSC_LCD_ChipSelect_Chip2_Selected,
} OSC_LCD_ChipSelect_Type;

typedef uint8_t  OSC_LCD_DataPageAddress_Type;
typedef uint8_t  OSC_LCD_ColumnAddress_Type;
typedef uint8_t  OSC_LCD_DataBus_OutputData_Type;
typedef uint8_t  OSC_LCD_DataBus_LcdStatus_Type;

#define  OSC_LCD_DATAPAGE_ADDRESS_INVALID     255
#define  OSC_LCD_COLUMN_ADDRESS_INVALID       255
#define  OSC_LCD_STATUS_INVALID               255

#define  OSC_LCD_DATAPAGE_ADDRESS_COUNT         8
#define  OSC_LCD_COLUMN_ADDRESS_COUNT          64

typedef struct {
  OSC_LCD_DataBus_Status_Type         dataBusStatus;
  OSC_LCD_DataBus_Direction_Type      dataBusDir;
  OSC_LCD_DataBus_CmdData_Type        dataBusCmdData;
  OSC_LCD_ChipSelect_Type             chipSelectStatus;
  OSC_LCD_DataPageAddress_Type        chip1_dataPageAddress;
  OSC_LCD_ColumnAddress_Type          chip1_columnAddress;
  OSC_LCD_DataPageAddress_Type        chip2_dataPageAddress;
  OSC_LCD_ColumnAddress_Type          chip2_columnAddress;
  OSC_LCD_DataBus_OutputData_Type     outData;
  OSC_LCD_DataBus_LcdStatus_Type      lcdStatus;
} OSC_LCD_Driver_State_Type;

/*========================================= DRIVER_VARIABLES =======================================*/

OSC_LCD_Driver_State_Type OSC_LCD_Driver_State;

/*===================================== TRANSPORT_STATUS_TYPES =====================================*/

typedef enum {
  OSC_LCD_TransportBufferElementStatus_Empty,
  OSC_LCD_TransportBufferElementStatus_Full
} OSC_LCD_TransportBufferElementStatus_Type;

typedef enum {
  OSC_LCD_TransportState_Idle,
  OSC_LCD_TransportState_PageSet,
  OSC_LCD_TransportState_ColumnSet
} OSC_LCD_TransportState_Type;

typedef struct {
  uint8_t*                             dataPtr;
  uint8_t                              dataLength;
  OSC_LCD_DataPageAddress_Type         dataPageAddress;
  OSC_LCD_ColumnAddress_Type           columnAddress;
  OSC_LCD_ChipSelect_Type              chipSelect;
} OSC_LCD_TransportBufferElement_Type;

typedef uint32_t OSC_LCD_TransportBufferElement_Index_Type;

#define OSC_LCD_TRANSPORTBUFFER_SIZE    32
#define OSC_LCD_TRANSPORTBUFFER_ELEMENT_NOEMPTY   (~0)

/*======================================= TRANSPORT_VARIABLES =====================================*/

OSC_LCD_TransportBufferElement_Type OSC_LCD_TransportBuffer[OSC_LCD_TRANSPORTBUFFER_SIZE];
OSC_LCD_TransportBufferElement_Index_Type OSC_LCD_TransportBufferElement_Active;
OSC_LCD_TransportBufferElement_Index_Type OSC_LCD_TransportBufferElement_FirstEmpty;

/*========================================= INITIALIZATION =========================================*/

void OSC_LCD_Init(void);
void OSC_LCD_Init_Driver(void);
void OSC_LCD_Init_Transport(void);
void OSC_Analog_Init_BacklightPWM(void);

/*============================================ BACKLIGHT ===========================================*/
void OSC_LCD_BacklightPWM_Set(uint32_t dutyCycle);

/*============================================= DRIVER =============================================*/

OSC_LCD_Err_Type OSC_LCD_Driver(void);

#define OSC_LCD_DRIVER_FLUSH()        while(OSC_LCD_Driver_State.dataBusStatus == OSC_LCD_DataBus_Status_Busy) /*Waits until the transmission or reception finishes*/
#define OSC_LCD_DRIVER_IS_LCD_BUSY()  (((OSC_LCD_Driver_State.lcdStatus & DT64128Q_LCD_STATUS_REGISTER_BUSY_BIT) == DT64128Q_LCD_STATUS_REGISTER_BUSY)?1:0)

void OSC_LCD_Driver_StartReadStatus(OSC_LCD_ChipSelect_Type chipSelect);
void OSC_LCD_Driver_StartWriteData(OSC_LCD_ChipSelect_Type chipSelect,OSC_LCD_DataPageAddress_Type data);
void OSC_LCD_Driver_StartWriteDataPage(OSC_LCD_ChipSelect_Type chipSelect,OSC_LCD_DataPageAddress_Type dataPage);
void OSC_LCD_Driver_StartWriteColumn(OSC_LCD_ChipSelect_Type chipSelect,OSC_LCD_ColumnAddress_Type column);

void OSC_LCD_Driver_WaitForLCD(OSC_LCD_ChipSelect_Type chipSelect);

OSC_LCD_DriverTransfer_Status_Type OSC_LCD_Driver_Transfer(OSC_LCD_ChipSelect_Type                   chipSelect,
                                                           OSC_LCD_DataPageAddress_Type              payload,
                                                           OSC_LCD_DriverTransfer_PayloadType_Type   payloadType);

/*============================================ TRANSPORT ===========================================*/

OSC_LCD_Err_Type OSC_LCD_Transport_Start(
                OSC_LCD_DataPageAddress_Type    dataPageAddress,
                OSC_LCD_ColumnAddress_Type      columnAddress,
                uint8_t*                        dataPtr,
                uint8_t                         dataLength
       );

OSC_LCD_Err_Type OSC_LCD_TransportLine_Start(
                OSC_LCD_DataPageAddress_Type    dataPageAddress,
                OSC_LCD_ColumnAddress_Type      columnAddress,
                uint8_t*                        dataPtr,
                uint8_t                         dataLength
       );

void OSC_LCD_Transport(void);

#endif /* OSCILLOSCOPELCD_H_ */
