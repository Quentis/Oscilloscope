#include "MonitorCfg.h"
#include "MonitorHardwareAbstraction.h"

#define   SYSTICK_PRIO        0x16

#define   SYSTICK_RELOAD      0x29040  /*1kHz*/

#define   MTR_SYSTICK_TIMER_BIT_LENGTH    30

const uint32_t  MTR_systemTimeMax = ((1 << MTR_SYSTICK_TIMER_BIT_LENGTH) - 1) - ((1 << MTR_SYSTICK_TIMER_BIT_LENGTH) % MTR_EVENT_LOWPRIO_POLLFREQ);

void MTR_HardwareInit(void)
{
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  SysTick->LOAD = SYSTICK_RELOAD & SysTick_LOAD_RELOAD_Msk;
  SCB->SHP[15] = (uint8_t)SYSTICK_PRIO;
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  //SysTick_Config(10);
}
