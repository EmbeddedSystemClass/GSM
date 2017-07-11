#ifndef __USART3_H__
#define __USART3_H__

#include	"Define.h"

#define Usart3TxQueueLen	1024
#define Usart3RxQueueLen	1024

void Usart3_Init(void);
void EnableUsart3TXInterrupt(void);
MyRes SendDataByUsart3(void * data, unsigned short len);
unsigned short RecvDataByUsart3(void * data, unsigned short len);

#endif

