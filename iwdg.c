#include "stm32f10x.h"
#include "iwdg.h"

//----------------------------------------------------
void initIWDG(void)
{
	RCC->CSR |= RCC_CSR_LSION;
	while(!(RCC->CSR & RCC_CSR_LSIRDY)){}
	IWDG->KR = 0xCCCC;
	IWDG->KR = 0x5555;
	IWDG->PR = 0x7;
	IWDG->RLR = 100;
	while (IWDG->SR)
  {
  }
	IWDG->KR = 0xAAAA;
}
//----------------------------------------------------
