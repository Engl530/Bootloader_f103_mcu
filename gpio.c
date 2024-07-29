#include "stm32f10x.h"
#include "gpio.h"
//	GPIO initialization
//----------------------------------------------------

void initGPIO(void)
{
	uint32_t tmpreg=0;
	
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);
	//Delay after an RCC peripheral clock enabling
	tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);
	//NOJTAG: JTAG-DP Disabled and SW-DP Enabled 
	CLEAR_BIT(AFIO->MAPR,AFIO_MAPR_SWJ_CFG);
  SET_BIT(AFIO->MAPR, AFIO_MAPR_SWJ_CFG_JTAGDISABLE);
	//	enable PORTB bus
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
	tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);

	//	PB5 (UART_DE) - output 
	GPIOB->CRL &= ~GPIO_CRL_CNF5;//	CNF5[1:0] = 00 -  General purpose output push-pul
	GPIOB->CRL |= GPIO_CRL_MODE5; //	MODE5[1:0] = 11 - Output mode, max speed 50 MHz
	GPIOB->ODR &= ~GPIO_ODR_ODR5;  // set PB5 output to 0;*/
	
	//	PB14 (GREEN_ON) - output 
	GPIOB->CRH &= ~GPIO_CRH_CNF14;	//	CNF14[1:0] = 00 - General purpose output push-pul
	GPIOB->CRH |= GPIO_CRH_MODE14; 	//	MODE14[1:0] = 00 - Output mode, max speed 50 MHz
	GPIOB->ODR |= GPIO_ODR_ODR14;  // set PB14 output to 0;
	
	
	//сделать инит для ножек юарта
	//	PB6 - TX; PB7 - RX
	//RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
	GPIOB->CRL |= (GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1);
	GPIOB->CRL &= ~(GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0);
	GPIOB->CRL |= GPIO_CRL_MODE6; 
	GPIOB->CRL &= ~GPIO_CRL_MODE7; 
	GPIOB->ODR |= GPIO_ODR_ODR7;
	//PB6 & PB7 alt mode

}
//----------------------------------------------------
void deinitGPIO(void)
{
	GPIOB->CRL = 0x44444444;
	GPIOB->CRH = 0x44444444;
	AFIO->MAPR &= ~AFIO_MAPR_USART1_REMAP;
	RCC->AHBENR &= ~RCC_APB2ENR_IOPBEN;
	RCC->APB2ENR &= ~RCC_APB2ENR_AFIOEN;

}
//----------------------------------------------------
