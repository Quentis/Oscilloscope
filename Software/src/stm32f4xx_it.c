#include "stm32f4xx_it.h"


/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

void TIM4_IRQHandler(void)
{
	// lekérdezzük, hogy mi okozta a megszakítást
	if ( TIM_GetITStatus(TIM4, TIM_IT_Update))
	{
		// invertáljuk a ledet
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		// töröljük a megszakítás jelzõ flaget
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
FunctionalState EnableTimer = ENABLE;
void EXTI0_IRQHandler(void)
{
	// lekérdezzük, hogy mi okozta a megszakítást
	if ( EXTI_GetITStatus(EXTI_Line0))
	{
		// negáljuk az állapotváltozónkat
		EnableTimer=!EnableTimer;
		// az állapotváltozónak megfelelõen engedélyezzük/tiltjuk a timert
		TIM_Cmd(TIM4, EnableTimer);
		// töröljük a megszakítást jelzõ flaget
		EXTI_ClearITPendingBit(EXTI_Line0);
	}

}


