#ifndef UART_H_
#define UART_H_

void initUART(void);
void deinitUART(void);
void UART_Transmit(unsigned char data);
void UART_TransmitString(unsigned char *data, unsigned short size);
unsigned char UART_Receive(unsigned char* ReceivedData);
#endif /* UART_H_ */
