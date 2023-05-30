/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"//#include "stm32f10x.h"
#include "User_systick.h"

__IO static uint32_t TimingDelay;

void delay_1ms(uint32_t delay_conter)
{
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
	TimingDelay = delay_conter;
	while(TimingDelay!=0);
}

void delay_10us(uint32_t delay_conter)
{ 
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
	TimingDelay = delay_conter;
	while(TimingDelay!=0);
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}



