#include "stm32f10x.h"
#include "tim.h"
//----------------------------------------------------

void initTIM(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	TIM2->CNT = 1;
	TIM2->PSC = 36000; //частота предделител€ 2к√ц 
	TIM2->ARR = 1001; //считаем до 1000, одновление таймера раз в 500мс
	TIM2->CR1 |= TIM_CR1_CEN;
}
//----------------------------------------------------
void deinitTIM(void)
{
	RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;
	TIM2->CR1 = 0;
	TIM2->CNT = 0;
	TIM2->PSC = 0;
	TIM2->ARR = 0xFFFF; //reset value
	
}
//----------------------------------------------------
