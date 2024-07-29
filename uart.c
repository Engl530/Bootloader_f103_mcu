#include "stm32f10x.h"
#include "uart.h"
#include "main.h"

//----------------------------------------------------

void initUART(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	USART1->BRR = (((39)<<4)|33); //clock_freq =72MHz  boudrate=115200 BRR = (((39)<<4)|6)
	//USART1->BRR = (((4)<<4)|34); //clock_freq =8MHz  boudrate=115200 BRR = (((4)<<4)|34)
	//USART1->BRR = (((3)<<4)|06); //clock_freq =5.6MHz  boudrate=115200 BRR = (((3)<<4)|06)
	USART1->CR2 &= ~USART_CR2_STOP;
	USART1->CR1 = USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE; 
}
//----------------------------------------------------
void deinitUART(void)
{
	USART1->CR1 = 0;
	USART1->CR2 = 0;
	USART1->BRR = 0;
	RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
}
//----------------------------------------------------
void UART_Transmit(unsigned char data)
{
	//uint32_t isr;   // working copy of uart_SR register
	//isr = USART1->ISR;      //wait for Tx empty
	while(!(USART1->SR & USART_SR_TXE)){}
	GPIOB->ODR |= GPIO_ODR_ODR5; //PB5 - set driver enable
	USART1->DR = data;
	//isr = USART1->ISR;      //wait for Tx empty
	while(!(USART1->SR & USART_SR_TC)){}
	GPIOB->ODR &= ~GPIO_ODR_ODR5; //PA - reset driver disable
} // End of UART_Transmit()
//----------------------------------------------------
unsigned char UART_Receive(unsigned char* ReceivedData)
{
  uint32_t isr;   // working copy of uart_SR register
  isr = USART1->SR;      
  while(!(isr & 0x20 )) isr = USART1->SR;  //wait for Rx full
  if((isr & 0x04)||(isr & 0x01)) //check if overrun or parity error
  {
    *ReceivedData = USART1->DR ;        //receive data to clear error
    //UART_Transmit(NACK);     //send NACK to host
    return 0;           //and return error
  }  
  *ReceivedData = USART1->DR;     //receive data
  
  return 1;       //and return no error
} // End of UART_Receive()
//----------------------------------------------------
void UART_TransmitString(unsigned char *data, unsigned short size){
	
	for(uint16_t i=0; i<size; i++){
		UART_Transmit(data[i]);
	}
}
//----------------------------------------------------
