#include "stm32f4xx.h"

int main(void)
{
  GPIO_InitTypeDef portinit;	//GPIO init strukt�ra
  TIM_TimeBaseInitTypeDef timerinit; // Timer init strukt�ra
  NVIC_InitTypeDef nvicinit; // NVIC init strukt�ra
  EXTI_InitTypeDef extiinit; // EXTI init strukt�ra

  // �rajelet adunk a GPIOA �s GPIOD portoknak
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOD, ENABLE);

  portinit.GPIO_Mode = GPIO_Mode_OUT;
  portinit.GPIO_OType = GPIO_OType_PP;
  portinit.GPIO_Pin = GPIO_Pin_12;
  portinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
  portinit.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOD, &portinit); // LED pin be�ll�t�sa


  GPIO_StructInit(&portinit);
  portinit.GPIO_Mode = GPIO_Mode_IN;
  portinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
  portinit.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOD, &portinit); // BTN pin be�ll�t�sa

  // �rajelet adunk a TIM4 id�z�t�nek
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  // Be�ll�tjuk az id�z�t� param�tereit
  TIM_TimeBaseStructInit(&timerinit);
  timerinit.TIM_CounterMode = TIM_CounterMode_Up; // felfel� sz�molunk
  timerinit.TIM_Period = 41999; // eddig sz�molunk
  timerinit.TIM_Prescaler = 999; // a busz �rajelet ennyivel osztjuk le
  TIM_TimeBaseInit(TIM4, &timerinit);

  // Emged�lyezz�k, hogy a TIM4 �jrat�lt�skor IT-t gener�ljon
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  // �rajelet adunk a SYSCFG eszk�znek (k�ls� interrupthoz kell)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // Enged�lyezz�k, hogy az A port 0-s l�ba k�ls� megszak�t�st gener�lhasson
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  // Be�ll�tjuk, hogy mire keletkezzen megszak�t�s
  extiinit.EXTI_Line = EXTI_Line0; // 0-s vonal
  extiinit.EXTI_LineCmd = ENABLE; // enged�lyez
  extiinit.EXTI_Mode = EXTI_Mode_Interrupt; // interrupt legyen
  extiinit.EXTI_Trigger = EXTI_Trigger_Rising; // felfut� �lre
  EXTI_Init(&extiinit);

  // Be�ll�tjuk a priorit�s modelt (1 bit csoport, 3 bit al priorit�s)
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  // TIM4 timer IT be�ll�t�sa
  nvicinit.NVIC_IRQChannel = TIM4_IRQn; // TIM4 IT
  nvicinit. NVIC_IRQChannelCmd = ENABLE; // enged�lyez
  nvicinit.NVIC_IRQChannelPreemptionPriority = 0; // legmagasabb csoport priorit�s
  nvicinit.NVIC_IRQChannelSubPriority = 0; // csoporton bel�l a legmagasabb priorit�s
  NVIC_Init(&nvicinit);

  // EXTI0 IT be�ll�t�sa
  nvicinit.NVIC_IRQChannel = EXTI0_IRQn; //EXTI0 vonal
  nvicinit. NVIC_IRQChannelCmd = ENABLE; // enged�lyez
  nvicinit.NVIC_IRQChannelPreemptionPriority = 1; // alacsony csoport priorit�s
  nvicinit.NVIC_IRQChannelSubPriority = 0; // csoporton bel�l a legmagasabb priorit�s
  NVIC_Init(&nvicinit);

  // TIM4 elind�t�sa
  TIM_Cmd(TIM4, ENABLE);

  while (1){}
}
