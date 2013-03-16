#include "stm32f4xx.h"

int main(void)
{
  GPIO_InitTypeDef portinit;	//GPIO init struktúra
  TIM_TimeBaseInitTypeDef timerinit; // Timer init struktúra
  NVIC_InitTypeDef nvicinit; // NVIC init struktúra
  EXTI_InitTypeDef extiinit; // EXTI init struktúra

  // Órajelet adunk a GPIOA és GPIOD portoknak
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOD, ENABLE);

  portinit.GPIO_Mode = GPIO_Mode_OUT;
  portinit.GPIO_OType = GPIO_OType_PP;
  portinit.GPIO_Pin = GPIO_Pin_12;
  portinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
  portinit.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOD, &portinit); // LED pin beállítása


  GPIO_StructInit(&portinit);
  portinit.GPIO_Mode = GPIO_Mode_IN;
  portinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
  portinit.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOD, &portinit); // BTN pin beállítása

  // Órajelet adunk a TIM4 idõzítõnek
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  // Beállítjuk az idõzítõ paramétereit
  TIM_TimeBaseStructInit(&timerinit);
  timerinit.TIM_CounterMode = TIM_CounterMode_Up; // felfelé számolunk
  timerinit.TIM_Period = 41999; // eddig számolunk
  timerinit.TIM_Prescaler = 999; // a busz órajelet ennyivel osztjuk le
  TIM_TimeBaseInit(TIM4, &timerinit);

  // Emgedélyezzük, hogy a TIM4 újratöltéskor IT-t generáljon
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  // Órajelet adunk a SYSCFG eszköznek (külsõ interrupthoz kell)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // Engedélyezzük, hogy az A port 0-s lába külsõ megszakítást generálhasson
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  // Beállítjuk, hogy mire keletkezzen megszakítás
  extiinit.EXTI_Line = EXTI_Line0; // 0-s vonal
  extiinit.EXTI_LineCmd = ENABLE; // engedélyez
  extiinit.EXTI_Mode = EXTI_Mode_Interrupt; // interrupt legyen
  extiinit.EXTI_Trigger = EXTI_Trigger_Rising; // felfutó élre
  EXTI_Init(&extiinit);

  // Beállítjuk a prioritás modelt (1 bit csoport, 3 bit al prioritás)
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  // TIM4 timer IT beállítása
  nvicinit.NVIC_IRQChannel = TIM4_IRQn; // TIM4 IT
  nvicinit. NVIC_IRQChannelCmd = ENABLE; // engedélyez
  nvicinit.NVIC_IRQChannelPreemptionPriority = 0; // legmagasabb csoport prioritás
  nvicinit.NVIC_IRQChannelSubPriority = 0; // csoporton belül a legmagasabb prioritás
  NVIC_Init(&nvicinit);

  // EXTI0 IT beállítása
  nvicinit.NVIC_IRQChannel = EXTI0_IRQn; //EXTI0 vonal
  nvicinit. NVIC_IRQChannelCmd = ENABLE; // engedélyez
  nvicinit.NVIC_IRQChannelPreemptionPriority = 1; // alacsony csoport prioritás
  nvicinit.NVIC_IRQChannelSubPriority = 0; // csoporton belül a legmagasabb prioritás
  NVIC_Init(&nvicinit);

  // TIM4 elindítása
  TIM_Cmd(TIM4, ENABLE);

  while (1){}
}
