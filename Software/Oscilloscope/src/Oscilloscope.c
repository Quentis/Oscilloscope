#include "Oscilloscope.h"
#include "MonitorInterface.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"
#include "USB_RxTx.h"

void Oscilloscope(void){
  uint8_t ch;

  MTR_Init();


  MTR_Alarm_Enable(MTR_ALARM_NAME_BUTTON_READ);
  MTR_Alarm_Enable(MTR_ALARM_NAME_DISPLAY_MANAGER);
  MTR_Alarm_Enable(MTR_ALARM_NAME_BUTTON_STATE_MACHINE);
  MTR_Alarm_Enable(MTR_ALARM_NAME_VIEW_EVENT_MANAGER);
  //MTR_Alarm_Enable(MTR_ALARM_NAME_TESTALARM_10ms);
  //MTR_Alarm_Enable(MTR_ALARM_NAME_TESTALARM_100ms);
  //MTR_Alarm_Enable(MTR_ALARM_NAME_TESTALARM_500ms);
  //MTR_Alarm_Enable(MTR_ALARM_NAME_TESTALARM_1000ms);
  OSC_DisplayManager_ClearScreen();

  while(1){
    if(USB_RxTx_rxFifoElementNum > 0){
      ch = USB_RxTx_getChar();
      USB_RxTx_sendChar(ch);
    }
    MTR_Scheduler();

    if(ADC_GetFlagStatus(ADC1,ADC_FLAG_OVR)) OSC_LED_RANGE_PLUS_SET();
    if(ADC_GetFlagStatus(ADC2,ADC_FLAG_OVR)) OSC_LED_RANGE_MINUS_SET();

  }
}

void OSC_TestAlarm_10ms(void){
  static int i = 0;
  char numStr[13] = "         000";
  numStr[11] = 48 + (i/100) % 10;
  numStr[10] = 48 + (i/1000) % 10;
  numStr[9] = 48 + (i/10000) % 10;
  if(!(i % 100)){
    OSC_DisplayManager_Text_PrintLine(numStr,4,OSC_DisplayManager_PrintMode_Negative);
  }
  i++;
  OSC_DisplayManager_Text_PrintChar(numStr[11],3,8);
  OSC_DisplayManager_Text_PrintChar(((numStr[11]- 48 + 1) % 10) + 48,3,9);
  OSC_DisplayManager_Text_PrintChar(((numStr[11]- 48 + 2) % 10) + 48,3,10);
  OSC_DisplayManager_Text_PrintChar(((numStr[11]- 48 + 3) % 10) + 48,3,11);
  OSC_DisplayManager_Text_PrintChar(((numStr[11]- 48 + 4) % 10) + 48,3,12);
}

void OSC_TestAlarm_100ms(void){
  STM32F4_Discovery_LEDToggle(LED4);
}

void OSC_TestAlarm_500ms(void){
  STM32F4_Discovery_LEDToggle(LED5);
}

void OSC_TestAlarm_1000ms(void){
  STM32F4_Discovery_LEDToggle(LED6);
  OSC_DSP_StartDataAcquisition();
}

void OSC_ClearLEDs(void){
  static uint32_t enable = 1;
  MTR_Event_Clear(MTR_EVENT_NAME_DISCOVERY_USER_BTN_PUSH);
  if(enable){
    MTR_Alarm_Disable(0);
    MTR_Alarm_Disable(1);
    MTR_Alarm_Disable(2);
    MTR_Alarm_Disable(3);
    STM32F4_Discovery_LEDOff(LED3);
    STM32F4_Discovery_LEDOff(LED4);
    STM32F4_Discovery_LEDOff(LED5);
    STM32F4_Discovery_LEDOff(LED6);
    enable = 0;
  } else {
    MTR_Alarm_Enable(0);
    MTR_Alarm_Enable(1);
    MTR_Alarm_Enable(2);
    MTR_Alarm_Enable(3);
    enable = 1;
  }
}
