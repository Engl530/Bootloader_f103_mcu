#include "stm32f10x.h"
#include "flash.h"

void Flash_Unlock(void)
{
	while ((FLASH->SR & FLASH_SR_BSY) != 0) //Wait till no operation is on going
	{
	 /* For robust implementation, add here time-out management */
	}
	if ((FLASH->CR & FLASH_CR_LOCK) != 0) //Check that the Flash is unlocked
	{
	 FLASH->KEYR = FLASH_KEY1; //Perform unlock sequence
	 FLASH->KEYR = FLASH_KEY2;
	}
}
//----------------------------------------------------
void Flash_Lock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}
//----------------------------------------------------
void WriteHalfWord(unsigned int flash_addr, unsigned short data)
{
	Flash_Unlock();
	FLASH->CR |= FLASH_CR_PG; //Set the PG bit in the FLASH_CR register to enable programming
	*(__IO uint16_t*)(flash_addr) = data; //  Perform the data write (half-word) at the desired address
	while ((FLASH->SR & FLASH_SR_BSY) != 0) //Wait until the BSY bit is reset in the FLASH_SR register 
	{
	/* For robust implementation, add here time-out management */
	} 
	if ((FLASH->SR & FLASH_SR_EOP) != 0) // Check the EOP flag in the FLASH_SR register
	{
	 FLASH->SR = FLASH_SR_EOP; //clear it by software by writing it at 1
	}
	else
	{
	 /* Manage the error cases */
	}
	FLASH->CR &= ~FLASH_CR_PG; //Reset the PG Bit to disable programming
	Flash_Lock();
}
//----------------------------------------------------
void WriteHexString(unsigned int adr, unsigned char* data)
{
	uint16_t* tmp_adr=(uint16_t*)adr;
	uint16_t tmp_data=0;
	Flash_Unlock();
	FLASH->CR |= FLASH_CR_PG; //Set the PG bit in the FLASH_CR register to enable programming
	for(unsigned char k=0; k<8; k++)
	{
		tmp_data = ((*(unsigned char*)(data+2*k+1)<<8)) + (*(unsigned char*)(data+2*k));
		*(tmp_adr++) = tmp_data;
		
		while ((FLASH->SR & FLASH_SR_BSY) != 0) //Wait until the BSY bit is reset in the FLASH_SR register 
		{
		/* For robust implementation, add here time-out management */
		} 
		if ((FLASH->SR & FLASH_SR_EOP) != 0) // Check the EOP flag in the FLASH_SR register
		{
		 FLASH->SR = FLASH_SR_EOP; //clear it by software by writing it at 1
		}
		else
		{
		 /* Manage the error cases */
		}
	}
	FLASH->CR &= ~FLASH_CR_PG; //Reset the PG Bit to disable programming
	Flash_Lock();
}
//----------------------------------------------------
void ErasePage(unsigned int flash_addr)
{
	Flash_Unlock();
	FLASH->CR |= FLASH_CR_PER; //Set the PER bit in the FLASH_CR register to enable page erasing
	FLASH->AR = flash_addr; // Program the FLASH_AR register to select a page to erase
	FLASH->CR |= FLASH_CR_STRT; //Set the STRT bit in the FLASH_CR register to start the erasing
	while ((FLASH->SR & FLASH_SR_BSY) != 0) //Wait until the BSY bit is reset in the FLASH_SR register
	{
	 /* For robust implementation, add here time-out management */
	}
	if ((FLASH->SR & FLASH_SR_EOP) != 0) //Check the EOP flag in the FLASH_SR register
	{
	 FLASH->SR = FLASH_SR_EOP; //Clear EOP flag by software by writing EOP at 1
	}
	else
	{
	 /* Manage the error cases */
	}
	FLASH->CR &= ~FLASH_CR_PER; // Reset the PER Bit to disable the page erase
	Flash_Lock();
}
//----------------------------------------------------
/*void OB_unlock(void)
{
	while ((FLASH->SR & FLASH_SR_BSY) != 0) // (1) Wait till no operation is on going 
	{
	 // For robust implementation, add here time-out management 
	}
	if ((FLASH->CR & FLASH_CR_LOCK) != 0) // (2) Check that the Flash is unlocked 
	{
	 FLASH->KEYR = FLASH_KEY1; // (3) Perform unlock sequence for Flash 
	 FLASH->KEYR = FLASH_KEY2;
	}
	if ((FLASH->CR & FLASH_CR_OPTWRE) == 0) // (4) Check that the Option Bytes are unlocked 
	{
	 FLASH->OPTKEYR = FLASH_OPTKEY1; // (5) Perform unlock sequence for Option Bytes 
	 FLASH->OPTKEYR = FLASH_OPTKEY2;
	}
}*/
//----------------------------------------------------
void OB_lock(void)
{
	FLASH->CR	= FLASH_CR_LOCK;
}
//----------------------------------------------------
/*void OB_programming(void)
{
	FLASH->CR |= FLASH_CR_OPTPG; // (1) Set the PG bit in the FLASH_CR register to enable programming 
	FLASH->OBR |= FLASH_OBR_RDPRT1 | FLASH_OBR_RDPRT2;
	//*opt_addr = data; // (2) Perform the data write 
	//OB->RDP |= OB_RDP_RDP;
	*(uint32_t*)(0x1FFFF800) =  0x00FF00FF;
	while ((FLASH->SR & FLASH_SR_BSY) != 0) // (3) Wait until the BSY bit is reset in the FLASH_SR register 
	{
	  For robust implementation, add here time-out management 
	}
	if ((FLASH->SR & FLASH_SR_EOP) != 0) // (4) Check the EOP flag in the FLASH_SR register 
	{
	 FLASH->SR = FLASH_SR_EOP; // (5) Clear the EOP flag by software by writing it at 1 
	}
	else
	{
	 // Manage the error cases 
	}
	FLASH->CR &= ~FLASH_CR_OPTPG; // (6) Reset the PG Bit to disable programming 
}*/
//----------------------------------------------------