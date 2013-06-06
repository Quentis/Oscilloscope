#include "OscilloscopeAccelerometer.h"

void OSC_Accelerometer_Init(void){
  uint8_t cfg;
  LIS302DL_InitTypeDef  initStruct = {
                            LIS302DL_LOWPOWERMODE_ACTIVE,
                            LIS302DL_DATARATE_100,
                            LIS302DL_XYZ_ENABLE,
                            LIS302DL_FULLSCALE_2_3,
                            LIS302DL_SELFTEST_NORMAL
                        };

  LIS302DL_FilterConfigTypeDef filterCfg = {
                                    LIS302DL_FILTEREDDATASELECTION_BYPASSED,
                                    LIS302DL_HIGHPASSFILTER_LEVEL_3,
                                    LIS302DL_HIGHPASSFILTERINTERRUPT_OFF
                               };

  LIS302DL_InterruptConfigTypeDef   itCfg = {
                                        LIS302DL_INTERRUPTREQUEST_NOTLATCHED,
                                        LIS302DL_CLICKINTERRUPT_XYZ_DISABLE,
                                        LIS302DL_DOUBLECLICKINTERRUPT_X_ENABLE | LIS302DL_DOUBLECLICKINTERRUPT_Y_ENABLE
                                    };
  GPIO_InitTypeDef  gpioInit = {
                        LIS302DL_ITSRC2_PIN,
                        GPIO_Mode_IN,
                        GPIO_Speed_50MHz,      /*Don't care*/
                        GPIO_OType_PP,        /*Don't care*/
                        GPIO_PuPd_NOPULL
                    };

  EXTI_InitTypeDef  extiInit = {
                        LIS302DL_ITSRC2_EXTI_LINE,
                        EXTI_Mode_Interrupt,
                        EXTI_Trigger_Rising,
                        ENABLE
                    };

  NVIC_InitTypeDef  nvicExtiInit = {
                        EXTI1_IRQn,
                        LIS302DL_ITSRC2_PRE_PRIO,
                        LIS302DL_ITSRC2_SUB_PRIO,
                        ENABLE
                    };
  NVIC_InitTypeDef  nvicSPIinit = {
                        SPI1_IRQn,
                        LIS302DL_ITSRC2_PRE_PRIO,
                        LIS302DL_ITSRC2_SUB_PRIO,
                        ENABLE
                    };

  RCC_AHB1PeriphClockCmd(LIS302DL_ITSRC2_CLK,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  GPIO_Init(LIS302DL_ITSRC2_PORT,&gpioInit);

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);

  EXTI_Init(&extiInit);
  NVIC_Init(&nvicExtiInit);

  LIS302DL_Init(&initStruct);
  LIS302DL_FilterConfig(&filterCfg);

  cfg = LIS302DL_CLICK_THSx | LIS302DL_CLICK_THSy;    /*These two threshold values are in the same register */
  LIS302DL_Write(&cfg,LIS302DL_CLICK_THSY_X_REG_ADDR,1);

  cfg = LIS302DL_CLICK_THSz;
  LIS302DL_Write(&cfg,LIS302DL_CLICK_THSZ_REG_ADDR,1);

  cfg = LIS302DL_CLICK_TimeLimit;
  LIS302DL_Write(&cfg,LIS302DL_CLICK_TIMELIMIT_REG_ADDR,1);

  cfg = LIS302DL_CLICK_Latency;
  LIS302DL_Write(&cfg,LIS302DL_CLICK_LATENCY_REG_ADDR,1);

  cfg = LIS302DL_CLICK_Window;
  LIS302DL_Write(&cfg,LIS302DL_CLICK_WINDOW_REG_ADDR,1);

  cfg = LIS302DL_CTRL_REG3_ITH_PUSHPULL_CLICK_ITSRC2;
  LIS302DL_Write(&cfg,LIS302DL_CTRL_REG3_ADDR,1);

  LIS302DL_InterruptConfig(&itCfg);

  NVIC_Init(&nvicSPIinit);
  SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);
}

#define LIS302DL_READ         0x80
#define DUMMY_BYTE_TO_READ    0

  static uint32_t dataPos = 0;
  static int8_t OSC_AccelerometerClickData[] = {0,0,0,0};

  static int8_t SPI1_ReceiveData = 0;

void OSC_Accelerometer_GetClickData(void){
  static OSC_AccStateType state = Osc_Accelerometer_State_Idle;
  uint32_t eventStatus = MTR_Event_GetStatus();
  MTR_Event_Clear(MTR_EVENT_NAME_CLICK_INTERRUPT | MTR_EVENT_NAME_CLICK_DATA_IN);

  if((eventStatus & MTR_EVENT_NAME_CLICK_INTERRUPT) && (state == Osc_Accelerometer_State_Idle)){
    state = Osc_Accelerometer_State_ClickEvent;
  }

  if((state == Osc_Accelerometer_State_ClickEvent) || (eventStatus & MTR_EVENT_NAME_CLICK_DATA_IN)){

      if(state == Osc_Accelerometer_State_ClickEvent){
        dataPos = OSC_ACCMETER_CLICK_DATA_CLICK_SRC;
        state = Osc_Accelerometer_State_StartAddressTransmit;
      }

      if(dataPos == OSC_ACCMETER_CLICK_DATA_CLICK_SRC){
        if(state == Osc_Accelerometer_State_StartAddressTransmit) {
          LIS302DL_CS_LOW();
          SPI_I2S_SendData(SPI1,(uint8_t)LIS302DL_CLICK_SRC_REG_ADDR | LIS302DL_READ);
          state = Osc_Accelerometer_State_StartDataReceive;
        } else if(state == Osc_Accelerometer_State_StartDataReceive) {
          SPI_I2S_SendData(SPI1,(uint8_t)DUMMY_BYTE_TO_READ);
          state = Osc_Accelerometer_State_DataRead;
        } else if(state == Osc_Accelerometer_State_DataRead){
          OSC_AccelerometerClickData[OSC_ACCMETER_CLICK_DATA_CLICK_SRC] = (int8_t)SPI1_ReceiveData;
          state = Osc_Accelerometer_State_StartAddressTransmit;
          dataPos = OSC_ACCMETER_CLICK_DATA_X_AXIS;
          LIS302DL_CS_HIGH();
        }
      }
      if(dataPos == OSC_ACCMETER_CLICK_DATA_X_AXIS){
        if(state == Osc_Accelerometer_State_StartAddressTransmit) {
          LIS302DL_CS_LOW();
          SPI_I2S_SendData(SPI1,(uint8_t)LIS302DL_OUT_X_ADDR | LIS302DL_READ);
          state = Osc_Accelerometer_State_StartDataReceive;
        } else if(state == Osc_Accelerometer_State_StartDataReceive) {
          SPI_I2S_SendData(SPI1,(uint8_t)DUMMY_BYTE_TO_READ);
          state = Osc_Accelerometer_State_DataRead;
        } else if(state == Osc_Accelerometer_State_DataRead){
          OSC_AccelerometerClickData[OSC_ACCMETER_CLICK_DATA_X_AXIS] = (int8_t)SPI1_ReceiveData;
          state = Osc_Accelerometer_State_StartAddressTransmit;
          dataPos = OSC_ACCMETER_CLICK_DATA_Y_AXIS;
          LIS302DL_CS_HIGH();
        }
      }
      if(dataPos == OSC_ACCMETER_CLICK_DATA_Y_AXIS){
        if(state == Osc_Accelerometer_State_StartAddressTransmit) {
          LIS302DL_CS_LOW();
          SPI_I2S_SendData(SPI1,(uint8_t)LIS302DL_OUT_Y_ADDR | LIS302DL_READ);
          state = Osc_Accelerometer_State_StartDataReceive;
        } else if(state == Osc_Accelerometer_State_StartDataReceive) {
          SPI_I2S_SendData(SPI1,(uint8_t)DUMMY_BYTE_TO_READ);
          state = Osc_Accelerometer_State_DataRead;
        } else if(state == Osc_Accelerometer_State_DataRead){
          OSC_AccelerometerClickData[OSC_ACCMETER_CLICK_DATA_Y_AXIS] = (int8_t)SPI1_ReceiveData;
          state = Osc_Accelerometer_State_StartAddressTransmit;
          dataPos = OSC_ACCMETER_CLICK_DATA_Z_AXIS;
          LIS302DL_CS_HIGH();
        }
      }
      if(dataPos == OSC_ACCMETER_CLICK_DATA_Z_AXIS){
        if(state == Osc_Accelerometer_State_StartAddressTransmit) {
          LIS302DL_CS_LOW();
          SPI_I2S_SendData(SPI1,(uint8_t)LIS302DL_OUT_Z_ADDR | LIS302DL_READ);
          state = Osc_Accelerometer_State_StartDataReceive;
        } else if(state == Osc_Accelerometer_State_StartDataReceive) {
          SPI_I2S_SendData(SPI1,(uint8_t)DUMMY_BYTE_TO_READ);
          state = Osc_Accelerometer_State_DataRead;
        } else if(state == Osc_Accelerometer_State_DataRead){
          OSC_AccelerometerClickData[OSC_ACCMETER_CLICK_DATA_Z_AXIS] = (int8_t)SPI1_ReceiveData;
          state = Osc_Accelerometer_State_DataProcessing;
          dataPos = OSC_ACCMETER_CLICK_DATA_CLICK_SRC;
          LIS302DL_CS_HIGH();
        }
      }
      if(state == Osc_Accelerometer_State_DataProcessing){
        OSC_Accelerometer_ProcessData();
        state = Osc_Accelerometer_State_Idle;
      }
  }
}

void OSC_Accelerometer_ProcessData(void){
  int8_t acc_x = OSC_AccelerometerClickData[OSC_ACCMETER_CLICK_DATA_X_AXIS];
  int8_t acc_y = OSC_AccelerometerClickData[OSC_ACCMETER_CLICK_DATA_Y_AXIS];
  if(acc_x > OSC_ACCMETER_THRESHOLD_X_AXIS  || acc_y > OSC_ACCMETER_THRESHOLD_Y_AXIS ||
     acc_x < -OSC_ACCMETER_THRESHOLD_X_AXIS || acc_y < OSC_ACCMETER_THRESHOLD_Y_AXIS){
    if((acc_x > 0 ? acc_x : -acc_x) > (acc_y > 0 ? acc_y : -acc_y)) { /*The absolute value of x acceleration is bigger*/
      if(acc_x > 0){
        OSC_View_Event_RegisterEvent(OSC_View_Event_TurnDown);
      } else {
        OSC_View_Event_RegisterEvent(OSC_View_Event_TurnUp);
      }
    } else {    /*The absolute value of y acceleration is bigger*/
      if(acc_y > 0){
        OSC_View_Event_RegisterEvent(OSC_View_Event_TurnLeft);
      } else {
        OSC_View_Event_RegisterEvent(OSC_View_Event_TurnRight);
      }
    }
  }
}

void EXTI1_IRQHandler(void)         /*Click interrupt from LIS302DL*/
{
  //STM32F4_Discovery_LEDToggle(LED5);
  MTR_Event_Set(MTR_EVENT_NAME_CLICK_INTERRUPT);
  EXTI_ClearITPendingBit(EXTI1_IRQn);
}

void SPI1_IRQHandler(void){        /*Read the registers of LIS302DL*/
  uint8_t status = SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_RXNE);
  if(status){
    MTR_Event_Set(MTR_EVENT_NAME_CLICK_DATA_IN);
    SPI1_ReceiveData = (int8_t)SPI_I2S_ReceiveData(SPI1);
  }
}
