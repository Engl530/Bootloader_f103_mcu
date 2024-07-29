#ifndef FLASH_H_
#define FLASH_H_

#define PAGE_SIZE					1024
#define PAGES							127	//количество страниц

void Flash_Lock(void);
void Flash_Unlock(void);
void WriteHalfWord(unsigned int flash_addr, unsigned short data);
void WriteHexString(unsigned int adr, unsigned char* data);
void ErasePage(unsigned int flash_addr);
void OB_unlock(void);
void OB_lock(void);
void OB_programming(void);
#endif /* FLASH_H_ */
