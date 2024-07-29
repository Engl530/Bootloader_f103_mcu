#include "stm32f10x.h"
#include "rcc.h"

//----------------------------------------------------

void initRCC(void)
{
	
	/*if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL) // (1) 
	{
	 RCC->CFGR &= (uint32_t) (~RCC_CFGR_SW); // (2) 
	 while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) // (3) 
	 {
	 // For robust implementation, add here time-out management 
	 }
	}
	RCC->CR &= (uint32_t)(~RCC_CR_PLLON);// (4) 
	while((RCC->CR & RCC_CR_PLLRDY) != 0) // (5) 
	{
	 // For robust implementation, add here time-out management 
	}
	RCC->CFGR = RCC->CFGR & (~RCC_CFGR_PLLMULL) | (RCC_CFGR_PLLMULL2); // (6) 
	RCC->CR |= RCC_CR_PLLON; // (7) 
	while((RCC->CR & RCC_CR_PLLRDY) == 0) // (8) 
	{
	 // For robust implementation, add here time-out management 
	}
	RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_PLL); // (9) 
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) // (10) 
	{
	 // For robust implementation, add here time-out management 
	}*/
	
}
//----------------------------------------------------
void deinitRCC(void)
{
	uint32_t tmp=RCC->CR;
	tmp	&=	0x0000FF00;	//reset values
	tmp	+=	0x00000083;
	RCC->CR = tmp;
	RCC->CFGR = 0;
}
//----------------------------------------------------
