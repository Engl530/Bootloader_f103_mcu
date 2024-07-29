//константы памяти
#define FLASH_START_ADR 	(unsigned int)0x08000000
#define APP_START_ADDRESS (unsigned int)0x08000C00
#define APP_FLAG					(unsigned int)0x87C0BD61
#define APP_START_PAGE		3


//diveci id
#define DEVICE_ID 0x410 //device id взят из даташита на STM32F103C8T6


//команды RS
#define ACK					0x79
#define NACK				0x1F
#define CMD_ERASE		0x43
#define CMD_GETID		0x02
#define CMD_WRITE		0x2B
#define CMD_RESET		0x3C
#define CMD_BOOT_ON	0x6E
#define MAGIC_WORD  0x4321ABCD


//константы буфера
/*посылка на прием всегда состоит из: 0-команда, 1-инв. команда, 2,3,4,5 - магическое слово, далее данные, последний байт црц */
#define COMMAND_INDEX				0
#define NEG_COMMAND_INDEX		1
#define MAGIC_WORD_INDEX		2
#define FIRST_DATA_INDEX		6
#define CMD_WRITE_SIZE	26 //1-команда, 1-команда инв., 4- маг.слово, 2-адрес, 16-данных, 2-црц


//
#define CPU_NAME		"STM32F103C8T6"
#define BOARD_NAME	"..."
#define UART_BUF_SIZE	40
#define BTL_VERSION		"1.2.00" //

//функции
void JumpToApp(void);
void CMD_ERASE_handle(void);
void CMD_GETID_handle(void);
void CMD_WRITE_handle(void);
void CMD_RESET_handle(void);
unsigned short CheckSumCalc(unsigned char* pData, unsigned char len);
unsigned char Recieve_message_withou_while(void);
unsigned char FindAppFlag(void);
unsigned char ReceiveMagicWord(void);
