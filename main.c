#include "stm32f10x.h"
#include "gpio.h"
#include "rcc.h"
#include "uart.h"
#include "main.h"
#include "iwdg.h"
#include "flash.h"
#include <string.h>
#include "tim.h"

uint32_t tmp1=0;
uint8_t uart_buf[UART_BUF_SIZE];
uint16_t tmp_chk1=0;
static uint8_t rcnt=0;

int main(void)
{
	//if((DBGMCU->IDCODE & DBGMCU_IDCODE_DEV_ID) != DEVICE_ID) while(1){}//проверка на случай заливки файла в другой проц
	__disable_irq();
		//проверка флага
	//JumpToApp();
	if (FindAppFlag()==0) //если да то программа записана 
	{
		initRCC();
		initGPIO();
		initUART();
		initTIM();
		
		while(TIM2->CNT<1000)
		{
			if(Recieve_message_withou_while() == 0) {goto boot_on;}
		}
		deinitRCC();
		deinitGPIO();
		deinitUART();
		deinitTIM();
		//если записана программа то прыжок
		//__enable_irq();
		JumpToApp();
	}
	else
	{
		//если нет то активируем режим бутлоадера
		initRCC();
		initGPIO();
		GPIOB->ODR |= GPIO_ODR_ODR14;
		initUART();
	}
	
	boot_on: 
	{
		deinitTIM();
		GPIOB->ODR |= GPIO_ODR_ODR14;
	}
	//UART_TransmitString(msg, sizeof(msg));
	while(1)
	{
		
		UART_Receive(&uart_buf[COMMAND_INDEX]);
		UART_Receive(&uart_buf[NEG_COMMAND_INDEX]);
		if(uart_buf[COMMAND_INDEX] == (uint8_t)(~uart_buf[NEG_COMMAND_INDEX]))
		{
			switch (uart_buf[COMMAND_INDEX])
      {
        case CMD_ERASE:			CMD_ERASE_handle(); break;
        case CMD_GETID:			CMD_GETID_handle(); break;
        case CMD_WRITE:			CMD_WRITE_handle(); break;
        case CMD_RESET:			CMD_RESET_handle(); break;
        default:						UART_Transmit(NACK);
      }
		}
		else UART_Transmit(NACK);
	}
}

//----------------------------------------------------
void JumpToApp(void)
{
	__set_MSP(*((__IO uint32_t*) APP_START_ADDRESS)); // Установка MSP
	uint32_t JumpAddress = *((__IO uint32_t*) (APP_START_ADDRESS + 4));
	void (*reset_handler)(void) = (void*)JumpAddress;
	reset_handler(); 
	while(1){}
}
//----------------------------------------------------
void CMD_ERASE_handle(void)
{
	uint16_t tmp=0;
	uint16_t tmp_crc=0;
	if(ReceiveMagicWord()){UART_Transmit(NACK); return;}
	UART_Receive(&uart_buf[FIRST_DATA_INDEX]);
	UART_Receive(&uart_buf[FIRST_DATA_INDEX+1]);
	UART_Receive(&uart_buf[FIRST_DATA_INDEX+2]);
	tmp_crc = CheckSumCalc(uart_buf, FIRST_DATA_INDEX+1);
	if(((uint16_t)(uart_buf[FIRST_DATA_INDEX+2]<<8)+uart_buf[FIRST_DATA_INDEX+1] )== tmp_crc)
	{
		if(uart_buf[FIRST_DATA_INDEX] > PAGES) //если < PAGES то стираем все страницы, если нет то только одну
		{
			
			tmp = (uint16_t)APP_START_ADDRESS / PAGE_SIZE;//находим номер начальной страницы
			for(uint8_t i=(uint8_t)tmp; i<PAGES; i++)
			{
				
				ErasePage(FLASH_START_ADR+i*(PAGE_SIZE));
			}
		}
		else if(uart_buf[FIRST_DATA_INDEX] < APP_START_PAGE)
		{
			UART_Transmit(NACK);
			return;
		}
		else
		{
			
			ErasePage(uart_buf[FIRST_DATA_INDEX]*PAGE_SIZE);
		}
		UART_Transmit(ACK);
		return;
	}
	else UART_Transmit(NACK);
	
}
//----------------------------------------------------
void CMD_GETID_handle(void)
{
	uint8_t len=0;
	uint16_t tmp_crc=0;
	if(ReceiveMagicWord()){UART_Transmit(NACK); return;}
	UART_Receive(&uart_buf[FIRST_DATA_INDEX]);
	UART_Receive(&uart_buf[FIRST_DATA_INDEX+1]);
	tmp_crc = CheckSumCalc(uart_buf, FIRST_DATA_INDEX);
	if(((uint16_t)(uart_buf[FIRST_DATA_INDEX+1]<<8)+uart_buf[FIRST_DATA_INDEX] )== tmp_crc)
	{
		memcpy(uart_buf, CPU_NAME , strlen(CPU_NAME));
		len += strlen(CPU_NAME);
		memcpy(&uart_buf[len], "\n" , 1);
		len++;
		memcpy(&uart_buf[len], BOARD_NAME , strlen(BOARD_NAME));
		len += strlen(BOARD_NAME);
		memcpy(&uart_buf[len], "\n" , 1);
		len++;
		memcpy(&uart_buf[len], BTL_VERSION , strlen(BTL_VERSION));
		len += strlen(BTL_VERSION);
		for(uint8_t j=0; j<len; j++)
		{
			UART_Transmit(uart_buf[j]);
		}
	}
	else UART_Transmit(NACK);
}
//----------------------------------------------------
void CMD_WRITE_handle(void)
{
	uint16_t tmp_crc=0;
	if(ReceiveMagicWord()){UART_Transmit(NACK); return;}
	for(uint8_t j=FIRST_DATA_INDEX; j<CMD_WRITE_SIZE; j++)
	{
		UART_Receive(&uart_buf[j]);
	}
	tmp_chk1 = CheckSumCalc(uart_buf, CMD_WRITE_SIZE-2);
	if(((uint16_t)(uart_buf[CMD_WRITE_SIZE-1]<<8)+uart_buf[CMD_WRITE_SIZE-2] )== tmp_chk1)
	{
		WriteHexString((((unsigned int)uart_buf[FIRST_DATA_INDEX]<<8) + (uart_buf[FIRST_DATA_INDEX+1])), &uart_buf[FIRST_DATA_INDEX+2]);
		UART_Transmit(ACK);
	}
	else UART_Transmit(NACK);
}
//----------------------------------------------------
void CMD_RESET_handle(void)
{
	uint16_t tmp_crc=0;
	if(ReceiveMagicWord()){UART_Transmit(NACK); return;}
	UART_Receive(&uart_buf[FIRST_DATA_INDEX]);
	UART_Receive(&uart_buf[FIRST_DATA_INDEX+1]);
	tmp_crc = CheckSumCalc(uart_buf, FIRST_DATA_INDEX);
	if(((uint16_t)(uart_buf[FIRST_DATA_INDEX+1]<<8)+uart_buf[FIRST_DATA_INDEX] )== tmp_crc)
	{
		UART_Transmit(ACK);
		initIWDG();
		while(1){}
	}
	else UART_Transmit(NACK);
}
//----------------------------------------------------
unsigned short CheckSumCalc(unsigned char* pData, unsigned char len)
{
	unsigned int crc16=0xFFFF;
  unsigned int ui16=0xA001;
  unsigned char flag=0;
  while (len--) 
  {
    ui16 = *pData++ ;
    crc16 ^= ui16;
    for( int i=0;i < 8;++i)
    {
      flag = (crc16 & 1);
      crc16 >>= 1;
      if (flag) 
      {
        crc16 ^= 0xA001;
      }
    }
  }
  return crc16;
}
//----------------------------------------------------
unsigned char Recieve_message_withou_while(void)
{
	

  static uint8_t FLASH_ERASE_MESSAGE[8] = {CMD_BOOT_ON, (uint8_t)(~CMD_BOOT_ON), (uint8_t)(MAGIC_WORD>>24), (uint8_t)(MAGIC_WORD>>16), (uint8_t)(MAGIC_WORD>>8), (uint8_t)(MAGIC_WORD), 0,0};
	static uint16_t tmp = 0x630E;
	FLASH_ERASE_MESSAGE[6] = tmp;
	FLASH_ERASE_MESSAGE[7] = (tmp&0xFF00)>>8;
	
	uint32_t sr;   // working copy of uart_SR register
  sr = USART1->SR;      //wait for Rx full
  if((sr & 0x20 /*RXNE*/)) //check if overrun or parity error
  {
    
      uart_buf[rcnt] = USART1->DR;     //receive data
      if(uart_buf[rcnt] == FLASH_ERASE_MESSAGE[rcnt]) 
      {
        rcnt++;
      }
			else {rcnt==0;}
      if (rcnt==8)      //если приняли 7 байт то проверить их
      {
        UART_Transmit(ACK);
				return 0;
      }
    
  }
	return 1;
}
//----------------------------------------------------
unsigned char FindAppFlag(void)
{
	
	uint32_t tmp_adr=APP_START_ADDRESS;
	for(uint16_t i=0; i<400; i++)
	{
		tmp1 = *((uint32_t*) tmp_adr);
		if(*((uint32_t*) tmp_adr) == APP_FLAG) return 0; //если нашелся то выходим, 0 - нашли флаг
		else tmp_adr+=4;
	}
	return 1;
}
//----------------------------------------------------
unsigned char ReceiveMagicWord(void)
{
	UART_Receive(&uart_buf[MAGIC_WORD_INDEX]);
	UART_Receive(&uart_buf[MAGIC_WORD_INDEX+1]);
	UART_Receive(&uart_buf[MAGIC_WORD_INDEX+2]);
	UART_Receive(&uart_buf[MAGIC_WORD_INDEX+3]);
	if (((uart_buf[MAGIC_WORD_INDEX]<<24) + (uart_buf[MAGIC_WORD_INDEX+1]<<16) + (uart_buf[MAGIC_WORD_INDEX+2]<<8) + (uart_buf[MAGIC_WORD_INDEX+3])) == MAGIC_WORD) return 0;
	else return 1;
}
//----------------------------------------------------
