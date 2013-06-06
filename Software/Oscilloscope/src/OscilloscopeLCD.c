#include "OscilloscopeLCD.h"

void OSC_LCD_Init(void){
  GPIO_InitTypeDef         GPIO_InitStructure;
  NVIC_InitTypeDef         NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_InitStructure;

  RCC_AHB1PeriphClockCmd(OSC_LCD_RW_GPIO_CLK   | OSC_LCD_EN_GPIO_CLK      | OSC_LCD_RST_GPIO_CLK     |
                         OSC_LCD_CS1_GPIO_CLK  | OSC_LCD_CS2_GPIO_CLK     | OSC_LCD_E_GPIO_CLK       |
                         OSC_LCD_DC_GPIO_CLK   | OSC_LCD_BL_PWM_GPIO_CLK  | OSC_LCD_DATABUS_GPIO_CLK,
                         ENABLE);

  RCC_APB1PeriphClockCmd(OSC_LCD_TIM_CLK,ENABLE);

  TIM_InitStructure.TIM_ClockDivision       =   0;
  TIM_InitStructure.TIM_Prescaler           =   0;
  TIM_InitStructure.TIM_Period              =   OSC_LCD_TIMER_ARR_VALUE;
  TIM_InitStructure.TIM_RepetitionCounter   =   0;
  TIM_InitStructure.TIM_CounterMode         =   TIM_CounterMode_Up;

  TIM_TimeBaseInit(OSC_LCD_TIM,&TIM_InitStructure);
  TIM_SelectOnePulseMode(OSC_LCD_TIM,TIM_OPMode_Single);
  TIM_ClearITPendingBit(OSC_LCD_TIM,TIM_IT_Update);

  NVIC_InitStructure.NVIC_IRQChannel                      =   OSC_LCD_TIM_IRQCHN;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    =   OSC_LCD_TIM_PREEMP_PRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority           =   OSC_LCD_TIM_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd                   =   ENABLE;

  NVIC_Init(&NVIC_InitStructure);
  TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_InitStructure.GPIO_Pin = OSC_LCD_RW_GPIO_PIN;
  GPIO_Init(OSC_LCD_RW_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_LCD_EN_GPIO_PIN;
  GPIO_Init(OSC_LCD_EN_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_LCD_RST_GPIO_PIN;
  GPIO_Init(OSC_LCD_RST_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_LCD_CS1_GPIO_PIN;
  GPIO_Init(OSC_LCD_CS1_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_LCD_CS2_GPIO_PIN;
  GPIO_Init(OSC_LCD_CS2_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_LCD_E_GPIO_PIN;
  GPIO_Init(OSC_LCD_E_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_LCD_DC_GPIO_PIN;
  GPIO_Init(OSC_LCD_DC_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = OSC_LCD_BL_PWM_GPIO_PIN;
  GPIO_Init(OSC_LCD_BL_PWM_GPIO_PORT,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;   /*To be on the safe side*/
  /*For the modification of the bus direction the mode should be changed only (the other values are right)*/

  GPIO_InitStructure.GPIO_Pin = OSC_LCD_DATABUS_GPIO_PINS;
  GPIO_Init(OSC_LCD_DATABUS_GPIO_PORT,&GPIO_InitStructure);

  /*Initialization of the Backlight driver*/
  OSC_Analog_Init_BacklightPWM();
  /*=================================Initialization_of_Communication_Layers=================================*/
  OSC_LCD_Init_Driver();
  OSC_LCD_Init_Transport();
}

void OSC_Analog_Init_BacklightPWM(void)
{
  GPIO_InitTypeDef GPIO_InitStructure = {
      OSC_LCD_BL_PWM_GPIO_PIN,
      GPIO_Mode_AF,
      GPIO_Speed_2MHz,
      GPIO_OType_PP,
      GPIO_PuPd_NOPULL
  };

  TIM_TimeBaseInitTypeDef TIM_Base_InitStructure = {
      OSC_LCD_BL_PWM_TIMER_PRELOAD,
      TIM_CounterMode_Up,
      OSC_LCD_BL_PWM_TIMER_PERIOD,
      OSC_LCD_BL_PWM_TIMER_CLOCK_DIV_0,
      OSC_LCD_BL_PWM_TIMER_REPETITION_NO
  };

  TIM_OCInitTypeDef TIM_OC_InitStructure = {
      TIM_OCMode_PWM1,
      TIM_OutputState_Enable,
      TIM_OutputNState_Disable,
      OSC_LCD_BL_PWM_TIMER_OC_COMPARE_PULSE,
      TIM_OCPolarity_High,
      TIM_OCNPolarity_Low,
      TIM_OCIdleState_Reset,
      TIM_OCNIdleState_Reset
  };

  TIM_BDTRInitTypeDef TIM_BDTR_Initstructure = {
      TIM_OSSRState_Enable,
      TIM_OSSIState_Enable,
      TIM_LOCKLevel_OFF,
      OSC_LCD_BL_PWM_TIMER_DEADTIME,
      TIM_Break_Disable,
      TIM_BreakPolarity_Low,
      TIM_AutomaticOutput_Enable
  };

  OSC_LCD_BL_PWM_TIMER_CLK_ENABLE(OSC_LCD_BL_PWM_TIMER_CLK,ENABLE);

  TIM_TimeBaseInit(OSC_LCD_BL_PWM_TIMER,&TIM_Base_InitStructure);
  OSC_LCD_BL_PWM_TIMER_OC_CHANNEL_INIT(OSC_LCD_BL_PWM_TIMER,&TIM_OC_InitStructure);
  TIM_BDTRConfig(OSC_LCD_BL_PWM_TIMER,&TIM_BDTR_Initstructure);

  TIM_ARRPreloadConfig(OSC_LCD_BL_PWM_TIMER,ENABLE);
  OSC_LCD_BL_PWM_TIMER_OC_PRELOAD_CONFIG(OSC_LCD_BL_PWM_TIMER,TIM_OCPreload_Enable);

  TIM_GenerateEvent(OSC_LCD_BL_PWM_TIMER,TIM_EventSource_COM);
  TIM_GenerateEvent(OSC_LCD_BL_PWM_TIMER,TIM_EventSource_Update);

  TIM_CCxCmd(OSC_LCD_BL_PWM_TIMER,OSC_LCD_BL_PWM_TIMER_OC_CHANNEL,TIM_CCx_Enable);
  TIM_Cmd(OSC_LCD_BL_PWM_TIMER,ENABLE);

  RCC_AHB1PeriphClockCmd(OSC_LCD_BL_PWM_TIMER_GPIO_CLK,ENABLE);

  GPIO_PinAFConfig(OSC_LCD_BL_PWM_TIMER_GPIO_PORT,
                   OSC_LCD_BL_PWM_TIMER_GPIO_PINSRC,
                   OSC_LCD_BL_PWM_TIMER_GPIO_AF);

  GPIO_Init(OSC_LCD_BL_PWM_TIMER_GPIO_PORT,&GPIO_InitStructure);
}

void OSC_LCD_Init_Driver(void){
  OSC_LCD_Driver_State.dataBusStatus     = OSC_LCD_DataBus_Status_Free;
  OSC_LCD_Driver_State.dataBusDir        = OSC_LCD_DataBus_Direction_Read;
  OSC_LCD_Driver_State.dataBusCmdData    = OSC_LCD_DataBus_CmdData_Command;
  OSC_LCD_Driver_State.chipSelectStatus  = OSC_LCD_ChipSelect_NoChip_Selected;
  OSC_LCD_Driver_State.lcdStatus               = OSC_LCD_STATUS_INVALID;
  OSC_LCD_Driver_State.chip1_dataPageAddress   = OSC_LCD_DATAPAGE_ADDRESS_INVALID;
  OSC_LCD_Driver_State.chip1_columnAddress     = OSC_LCD_COLUMN_ADDRESS_INVALID;
  OSC_LCD_Driver_State.chip2_dataPageAddress   = OSC_LCD_DATAPAGE_ADDRESS_INVALID;
  OSC_LCD_Driver_State.chip2_columnAddress     = OSC_LCD_COLUMN_ADDRESS_INVALID;


  OSC_LCD_DATABUS_HW_DIR_SET_READ();
  OSC_LCD_RST_BIT_SET();
  OSC_LCD_CS1_BIT_SET();
  OSC_LCD_CS2_BIT_SET();
  OSC_LCD_E_BIT_CLEAR();
  OSC_LCD_DC_BIT_CLEAR();
  OSC_LCD_EN_BIT_CLEAR();

  OSC_LCD_Driver_State.outData = OSC_LCD_CMD_ON_OFF | OSC_LCD_CMD_ON;

  OSC_LCD_Driver_State.dataBusDir        = OSC_LCD_DataBus_Direction_Write;
  OSC_LCD_Driver_State.dataBusCmdData    = OSC_LCD_DataBus_CmdData_Command;
  OSC_LCD_Driver_State.chipSelectStatus  = OSC_LCD_ChipSelect_Chip1_Selected;
  OSC_LCD_Driver();
  OSC_LCD_DRIVER_FLUSH();
  OSC_LCD_Driver_WaitForLCD(OSC_LCD_ChipSelect_Chip1_Selected);

  OSC_LCD_Driver_State.dataBusDir        = OSC_LCD_DataBus_Direction_Write;
  OSC_LCD_Driver_State.dataBusCmdData    = OSC_LCD_DataBus_CmdData_Command;
  OSC_LCD_Driver_State.chipSelectStatus  = OSC_LCD_ChipSelect_Chip2_Selected;
  OSC_LCD_Driver();
  OSC_LCD_DRIVER_FLUSH();
  OSC_LCD_Driver_WaitForLCD(OSC_LCD_ChipSelect_Chip2_Selected);

  OSC_LCD_Driver_State.chipSelectStatus  = OSC_LCD_ChipSelect_NoChip_Selected;
  OSC_LCD_Driver_State.dataBusDir        = OSC_LCD_DataBus_Direction_Read;
}

void OSC_LCD_Init_Transport(void){
  OSC_LCD_TransportBufferElement_Active = 0;
  OSC_LCD_TransportBufferElement_FirstEmpty = 0;

  OSC_LCD_DRIVER_FLUSH();
  OSC_LCD_Driver_StartWriteColumn(OSC_LCD_ChipSelect_Chip1_Selected,0);
  OSC_LCD_Driver_WaitForLCD(OSC_LCD_ChipSelect_Chip1_Selected);
  OSC_LCD_Driver_StartWriteColumn(OSC_LCD_ChipSelect_Chip2_Selected,0);
  OSC_LCD_Driver_WaitForLCD(OSC_LCD_ChipSelect_Chip2_Selected);
  OSC_LCD_Driver_StartWriteDataPage(OSC_LCD_ChipSelect_Chip1_Selected,0);
  OSC_LCD_Driver_WaitForLCD(OSC_LCD_ChipSelect_Chip1_Selected);
  OSC_LCD_Driver_StartWriteDataPage(OSC_LCD_ChipSelect_Chip2_Selected,0);
  OSC_LCD_Driver_WaitForLCD(OSC_LCD_ChipSelect_Chip2_Selected);
}

void OSC_LCD_BacklightPWM_Set(uint32_t dutyCycle){
  OSC_LCD_BL_PWM_TIMER_OC_COMPARE_SET(OSC_LCD_BL_PWM_TIMER,dutyCycle);
}

OSC_LCD_Err_Type OSC_LCD_Driver(void){
  if(OSC_LCD_Driver_State.dataBusStatus    == OSC_LCD_DataBus_Status_Busy)        return OSC_LCD_Err_DataBus_Busy;
  if(OSC_LCD_Driver_State.chipSelectStatus == OSC_LCD_ChipSelect_NoChip_Selected) return OSC_LCD_Err_NoChip_Selected;

  if(OSC_LCD_Driver_State.dataBusCmdData == OSC_LCD_DataBus_CmdData_Data && OSC_LCD_Driver_State.dataBusDir == OSC_LCD_DataBus_Direction_Write){
    if(OSC_LCD_Driver_State.chipSelectStatus == OSC_LCD_ChipSelect_Chip1_Selected){
      OSC_LCD_Driver_State.chip1_columnAddress = (OSC_LCD_Driver_State.chip1_columnAddress + 1) & (DT64128Q_LCD_DATA_COLUMN_COUNT - 1);
    }
    if(OSC_LCD_Driver_State.chipSelectStatus == OSC_LCD_ChipSelect_Chip2_Selected){
      OSC_LCD_Driver_State.chip2_columnAddress = (OSC_LCD_Driver_State.chip2_columnAddress + 1) & (DT64128Q_LCD_DATA_COLUMN_COUNT - 1);
    }
  }

  OSC_LCD_Driver_State.dataBusStatus = OSC_LCD_DataBus_Status_Busy;
  OSC_LCD_E_BIT_CLEAR();

  if(OSC_LCD_Driver_State.dataBusDir == OSC_LCD_DataBus_Direction_Write){
    OSC_LCD_DATABUS_HW_DIR_SET_WRITE();
  } else {
    OSC_LCD_DATABUS_HW_DIR_SET_READ();
  }

  if(OSC_LCD_Driver_State.chipSelectStatus  == OSC_LCD_ChipSelect_Chip1_Selected){
    OSC_LCD_CS1_BIT_CLEAR();
    OSC_LCD_CS2_BIT_SET();
  } else if(OSC_LCD_Driver_State.chipSelectStatus  == OSC_LCD_ChipSelect_Chip2_Selected){
    OSC_LCD_CS1_BIT_SET();
    OSC_LCD_CS2_BIT_CLEAR();
  }

  if(OSC_LCD_Driver_State.dataBusCmdData == OSC_LCD_DataBus_CmdData_Command) OSC_LCD_DC_BIT_CLEAR();
  else OSC_LCD_DC_BIT_SET();

  OSC_LCD_TIM_WAIT_1US();
  return OSC_LCD_Err_OK;
}

void TIM6_DAC_IRQHandler(void){
  static uint8_t timingPhase = 1;   /*Number of microseconds since the start of read or write operation*/
  switch(timingPhase){

    case 1:
      OSC_LCD_E_BIT_SET();
      timingPhase++;
      if(OSC_LCD_Driver_State.dataBusDir == OSC_LCD_DataBus_Direction_Write){
        OSC_LCD_DATABUS_WRITE(OSC_LCD_Driver_State.outData);
      }
      OSC_LCD_TIM_WAIT_1US();
      break;

    case 2:
      OSC_LCD_E_BIT_CLEAR();
      timingPhase = 1;
      if(OSC_LCD_Driver_State.dataBusDir == OSC_LCD_DataBus_Direction_Read){
        OSC_LCD_Driver_State.lcdStatus = OSC_LCD_DATABUS_READ();
      }
      OSC_LCD_CS1_BIT_SET();
      OSC_LCD_CS2_BIT_SET();
      OSC_LCD_Driver_State.dataBusStatus = OSC_LCD_DataBus_Status_Free;
      break;

    default:
      timingPhase = 1;
      OSC_LCD_CS1_BIT_SET();
      OSC_LCD_CS2_BIT_SET();
      OSC_LCD_E_BIT_CLEAR();
      OSC_LCD_Driver_State.dataBusStatus = OSC_LCD_DataBus_Status_Free;
      break;
  }

  TIM_ClearITPendingBit(OSC_LCD_TIM,TIM_IT_Update);
}

void OSC_LCD_Driver_StartReadStatus(OSC_LCD_ChipSelect_Type chipSelect){
  if(OSC_LCD_Driver_State.dataBusStatus == OSC_LCD_DataBus_Status_Busy) return;
  OSC_LCD_Driver_State.dataBusCmdData   = OSC_LCD_DataBus_CmdData_Command;
  OSC_LCD_Driver_State.dataBusDir       = OSC_LCD_DataBus_Direction_Read;
  OSC_LCD_Driver_State.chipSelectStatus = chipSelect;
  OSC_LCD_Driver();
}

void OSC_LCD_Driver_StartWriteData(OSC_LCD_ChipSelect_Type chipSelect,OSC_LCD_DataPageAddress_Type data){
  if(OSC_LCD_Driver_State.dataBusStatus == OSC_LCD_DataBus_Status_Busy) return;

  OSC_LCD_Driver_State.dataBusCmdData   = OSC_LCD_DataBus_CmdData_Data;
  OSC_LCD_Driver_State.dataBusDir       = OSC_LCD_DataBus_Direction_Write;
  OSC_LCD_Driver_State.chipSelectStatus = chipSelect;
  OSC_LCD_Driver_State.outData          = data;
  OSC_LCD_Driver();
}

void OSC_LCD_Driver_StartWriteDataPage(OSC_LCD_ChipSelect_Type chipSelect,OSC_LCD_DataPageAddress_Type dataPage){
  if(OSC_LCD_Driver_State.dataBusStatus == OSC_LCD_DataBus_Status_Busy) return;
  if(chipSelect == OSC_LCD_ChipSelect_Chip1_Selected){
    OSC_LCD_Driver_State.chip1_columnAddress = dataPage;
  } else if(chipSelect == OSC_LCD_ChipSelect_Chip2_Selected) {
    OSC_LCD_Driver_State.chip2_columnAddress = dataPage;
  }
  OSC_LCD_Driver_State.dataBusCmdData   = OSC_LCD_DataBus_CmdData_Command;
  OSC_LCD_Driver_State.dataBusDir       = OSC_LCD_DataBus_Direction_Write;
  OSC_LCD_Driver_State.chipSelectStatus = chipSelect;
  OSC_LCD_Driver_State.outData          = (OSC_LCD_CMD_DATAPAGE | (dataPage & OSC_LCD_CMD_DATAPAGE_MASK));
  OSC_LCD_Driver();
}

void OSC_LCD_Driver_StartWriteColumn(OSC_LCD_ChipSelect_Type chipSelect,OSC_LCD_ColumnAddress_Type column){
  if(OSC_LCD_Driver_State.dataBusStatus == OSC_LCD_DataBus_Status_Busy) return;
  if(chipSelect == OSC_LCD_ChipSelect_Chip1_Selected){
    OSC_LCD_Driver_State.chip1_columnAddress = column;
  } else if(chipSelect == OSC_LCD_ChipSelect_Chip2_Selected) {
    OSC_LCD_Driver_State.chip2_columnAddress = column;
  }
  OSC_LCD_Driver_State.dataBusCmdData   = OSC_LCD_DataBus_CmdData_Command;
  OSC_LCD_Driver_State.dataBusDir       = OSC_LCD_DataBus_Direction_Write;
  OSC_LCD_Driver_State.chipSelectStatus = chipSelect;
  OSC_LCD_Driver_State.outData          = (OSC_LCD_CMD_COLUMNADDRESS | (column & OSC_LCD_CMD_COLUMNADDRESS_MASK));
  OSC_LCD_Driver();
}

void OSC_LCD_Driver_WaitForLCD(OSC_LCD_ChipSelect_Type chipSelect){
  do{
    OSC_LCD_Driver_StartReadStatus(chipSelect);
    OSC_LCD_DRIVER_FLUSH();
  } while(OSC_LCD_DRIVER_IS_LCD_BUSY());
}

OSC_LCD_DriverTransfer_Status_Type OSC_LCD_Driver_Transfer(OSC_LCD_ChipSelect_Type                   chipSelect,
                                                           OSC_LCD_DataPageAddress_Type              payload,
                                                           OSC_LCD_DriverTransfer_PayloadType_Type   payloadType){
  static OSC_LCD_DriverTransfer_InnerState_Type OSC_LCD_DriverTransfer_InnerState = OSC_LCD_DriverTransfer_InnerState_StartToTransmit;

  if(OSC_LCD_Driver_State.dataBusStatus == OSC_LCD_DataBus_Status_Free){
    switch(OSC_LCD_DriverTransfer_InnerState){
      case OSC_LCD_DriverTransfer_InnerState_StartToTransmit:
        switch(payloadType){
          case OSC_LCD_DriverTransfer_PayloadType_DataPageAddress:
            OSC_LCD_Driver_StartWriteDataPage(chipSelect,payload);
            break;
          case OSC_LCD_DriverTransfer_PayloadType_ColumnAddress:
            OSC_LCD_Driver_StartWriteColumn(chipSelect,payload);
            break;
          case OSC_LCD_DriverTransfer_PayloadType_Data:
            OSC_LCD_Driver_StartWriteData(chipSelect,payload);
            break;
        }
        OSC_LCD_DriverTransfer_InnerState = OSC_LCD_DriverTransfer_InnerState_PayloadSent;
        return OSC_LCD_DriverTransfer_Status_InProgress;
      case OSC_LCD_DriverTransfer_InnerState_PayloadSent:
        OSC_LCD_Driver_StartReadStatus(chipSelect);
        OSC_LCD_DriverTransfer_InnerState = OSC_LCD_DriverTransfer_InnerState_StatusRead;
        return OSC_LCD_DriverTransfer_Status_InProgress;
      case OSC_LCD_DriverTransfer_InnerState_StatusRead:
        if(OSC_LCD_DRIVER_IS_LCD_BUSY()){
          OSC_LCD_Driver_StartReadStatus(chipSelect); /*The state doesn't change because the LCD is still busy*/
          return OSC_LCD_DriverTransfer_Status_InProgress;
        } else {
          OSC_LCD_DriverTransfer_InnerState = OSC_LCD_DriverTransfer_InnerState_StartToTransmit;
          return OSC_LCD_DriverTransfer_Status_Completed;
        }
    }
  }
  return OSC_LCD_DriverTransfer_Status_InProgress;
}

OSC_LCD_Err_Type OSC_LCD_Transport_Start(
                OSC_LCD_DataPageAddress_Type    dataPageAddress,
                OSC_LCD_ColumnAddress_Type      columnAddress,
                uint8_t*                        dataPtr,
                uint8_t                         dataLength)
{
  if((dataPtr == 0) || (dataLength == 0) || (dataPageAddress >= OSC_LCD_DATAPAGE_ADDRESS_COUNT)) return OSC_LCD_Err_InvalidParameter;
  if(((columnAddress < OSC_LCD_COLUMN_ADDRESS_COUNT)  && ((columnAddress + dataLength) > OSC_LCD_COLUMN_ADDRESS_COUNT)) ||
     ((columnAddress >= OSC_LCD_COLUMN_ADDRESS_COUNT) && ((columnAddress + dataLength - OSC_LCD_COLUMN_ADDRESS_COUNT) > OSC_LCD_COLUMN_ADDRESS_COUNT)))
      return OSC_LCD_Err_InvalidParameter;

  if(((OSC_LCD_TransportBufferElement_FirstEmpty + 1) % OSC_LCD_TRANSPORTBUFFER_SIZE) != OSC_LCD_TransportBufferElement_Active) {

    OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_FirstEmpty].dataPageAddress   = dataPageAddress;
    OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_FirstEmpty].dataPtr           = dataPtr;
    OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_FirstEmpty].dataLength        = dataLength;
    OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_FirstEmpty].chipSelect        = (columnAddress < OSC_LCD_COLUMN_ADDRESS_COUNT)?
                                                                                            OSC_LCD_ChipSelect_Chip1_Selected:
                                                                                            OSC_LCD_ChipSelect_Chip2_Selected;
    if(columnAddress >= OSC_LCD_COLUMN_ADDRESS_COUNT) columnAddress -= OSC_LCD_COLUMN_ADDRESS_COUNT;
    columnAddress = OSC_LCD_COLUMN_ADDRESS_COUNT - columnAddress - dataLength;
    OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_FirstEmpty].columnAddress     = columnAddress;

    OSC_LCD_TransportBufferElement_FirstEmpty = (OSC_LCD_TransportBufferElement_FirstEmpty + 1) % OSC_LCD_TRANSPORTBUFFER_SIZE;
    return OSC_LCD_Err_OK;
  } else {
    return OSC_LCD_Err_BufferFull;
  }
}

OSC_LCD_Err_Type OSC_LCD_TransportLine_Start(
    OSC_LCD_DataPageAddress_Type    dataPageAddress,
    OSC_LCD_ColumnAddress_Type      columnAddress,
    uint8_t*                        dataPtr,
    uint8_t                         dataLength)
{
  if((dataPtr == 0) || (dataLength == 0) || (dataPageAddress >= OSC_LCD_DATAPAGE_ADDRESS_COUNT)) return OSC_LCD_Err_InvalidParameter;
  if((columnAddress + dataLength) > OSC_LCD_COLUMN_ADDRESS_COUNT * 2)
      return OSC_LCD_Err_InvalidParameter;
  if( (((OSC_LCD_TransportBufferElement_FirstEmpty + 1) % OSC_LCD_TRANSPORTBUFFER_SIZE) == OSC_LCD_TransportBufferElement_Active) ||
      (((OSC_LCD_TransportBufferElement_FirstEmpty + 2) % OSC_LCD_TRANSPORTBUFFER_SIZE) == OSC_LCD_TransportBufferElement_Active) ){
    return OSC_LCD_Err_BufferFull;
  }

  if(columnAddress < OSC_LCD_COLUMN_ADDRESS_COUNT){
    if(columnAddress + dataLength <= OSC_LCD_COLUMN_ADDRESS_COUNT){
      OSC_LCD_Transport_Start(dataPageAddress,columnAddress,dataPtr,dataLength);
    } else {
      OSC_LCD_Transport_Start(dataPageAddress,columnAddress,dataPtr,OSC_LCD_COLUMN_ADDRESS_COUNT - columnAddress);
      OSC_LCD_Transport_Start(dataPageAddress,OSC_LCD_COLUMN_ADDRESS_COUNT,
                              &dataPtr[OSC_LCD_COLUMN_ADDRESS_COUNT - columnAddress],
                              dataLength - (OSC_LCD_COLUMN_ADDRESS_COUNT - columnAddress));
    }
  } else {
    OSC_LCD_Transport_Start(dataPageAddress,columnAddress,dataPtr,dataLength);
  }
  return OSC_LCD_Err_OK;
}

void OSC_LCD_Transport(void){
  static OSC_LCD_TransportState_Type OSC_LCD_TransportState = OSC_LCD_TransportState_Idle;
  OSC_LCD_DriverTransfer_Status_Type transferStatus = OSC_LCD_DriverTransfer_Status_InProgress;
  uint8_t dataLength;

  switch(OSC_LCD_TransportState){
  case OSC_LCD_TransportState_Idle:
    if(OSC_LCD_TransportBufferElement_Active != OSC_LCD_TransportBufferElement_FirstEmpty){

      if(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].chipSelect == OSC_LCD_ChipSelect_Chip1_Selected){
        if(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].dataPageAddress == OSC_LCD_Driver_State.chip1_dataPageAddress){
          if(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].columnAddress == OSC_LCD_Driver_State.chip1_columnAddress){
            OSC_LCD_TransportState = OSC_LCD_TransportState_ColumnSet;
          } else {
            OSC_LCD_TransportState = OSC_LCD_TransportState_PageSet;
          }
        } else {
          transferStatus = OSC_LCD_Driver_Transfer(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].chipSelect,
                                                   OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].dataPageAddress,
                                                   OSC_LCD_DriverTransfer_PayloadType_DataPageAddress);
          if(transferStatus == OSC_LCD_DriverTransfer_Status_Completed){
            OSC_LCD_Driver_State.chip1_dataPageAddress = OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].dataPageAddress;
            OSC_LCD_TransportState = OSC_LCD_TransportState_PageSet;
          }
        }
      } else if(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].chipSelect == OSC_LCD_ChipSelect_Chip2_Selected){
        if(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].dataPageAddress == OSC_LCD_Driver_State.chip2_dataPageAddress){
          if(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].columnAddress == OSC_LCD_Driver_State.chip2_columnAddress){
            OSC_LCD_TransportState = OSC_LCD_TransportState_ColumnSet;
          } else {
            OSC_LCD_TransportState = OSC_LCD_TransportState_PageSet;
          }
        } else {
          transferStatus = OSC_LCD_Driver_Transfer(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].chipSelect,
                                                   OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].dataPageAddress,
                                                   OSC_LCD_DriverTransfer_PayloadType_DataPageAddress);
          if(transferStatus == OSC_LCD_DriverTransfer_Status_Completed){
            OSC_LCD_Driver_State.chip2_dataPageAddress = OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].dataPageAddress;
            OSC_LCD_TransportState = OSC_LCD_TransportState_PageSet;
          }
        }
      }
    }
    break;

  case OSC_LCD_TransportState_PageSet:
    transferStatus = OSC_LCD_Driver_Transfer(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].chipSelect,
                                             OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].columnAddress,
                                             OSC_LCD_DriverTransfer_PayloadType_ColumnAddress);

    if(transferStatus == OSC_LCD_DriverTransfer_Status_Completed){
      if(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].chipSelect == OSC_LCD_ChipSelect_Chip1_Selected){
        OSC_LCD_Driver_State.chip1_columnAddress = OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].columnAddress;
      } else {
        OSC_LCD_Driver_State.chip2_columnAddress = OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].columnAddress;
      }
      OSC_LCD_TransportState = OSC_LCD_TransportState_ColumnSet;
    }
    break;
  case OSC_LCD_TransportState_ColumnSet:
    dataLength = OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].dataLength;
    transferStatus = OSC_LCD_Driver_Transfer(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].chipSelect,
                                             OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].dataPtr[dataLength-1],
                                             OSC_LCD_DriverTransfer_PayloadType_Data);

    if(transferStatus == OSC_LCD_DriverTransfer_Status_Completed){
      if(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].chipSelect == OSC_LCD_ChipSelect_Chip1_Selected){
        OSC_LCD_Driver_State.chip1_columnAddress = (OSC_LCD_Driver_State.chip1_columnAddress + 1) % OSC_LCD_COLUMN_ADDRESS_COUNT;
      } else {
        OSC_LCD_Driver_State.chip2_columnAddress = (OSC_LCD_Driver_State.chip2_columnAddress + 1) % OSC_LCD_COLUMN_ADDRESS_COUNT;
      }
      OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].dataLength--;
      if(OSC_LCD_TransportBuffer[OSC_LCD_TransportBufferElement_Active].dataLength == 0){
        OSC_LCD_TransportBufferElement_Active = (OSC_LCD_TransportBufferElement_Active + 1) % OSC_LCD_TRANSPORTBUFFER_SIZE;
        OSC_LCD_TransportState = OSC_LCD_TransportState_Idle;
      }
    }
    break;
  }
}

