/***************************************************************************************************
*FileName:Usart3_Driver
*Description:串口3驱动
*Author:xsx
*Data:2016年4月29日11:50:01
***************************************************************************************************/






/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include 	"stm32l1xx.h"
#include 	"Usart3_Driver.h"

#include "MyQueue.h"
#include 	"stdio.h"
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
static QUEUE8_t usart3TxQueue;								//串口2发送队列
static unsigned char usart3TxQueueBuffer[Usart3TxQueueLen];	//串口2发送队列数据域

static QUEUE8_t usart3RxQueue;								//串口2接收队列
static unsigned char usart3RxQueueBuffer[Usart3RxQueueLen];	//串口2接收队列数据域
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
static void usart3QueueInit(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName：ConfigUsart3
*Description：串口3的端口初始化和配置
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:52:08
***************************************************************************************************/
void Usart3_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 开启GPIO_D的时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* 开启串口3的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	USART_InitStructure.USART_BaudRate   = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART3, &USART_InitStructure);

	/* 使能串口2 */
	USART_Cmd(USART3, ENABLE);
	//使能接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	/* NVIC configuration */
	/* Configure the Priority Group to 2 bits */


	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	usart3QueueInit();
}

static void usart3QueueInit(void)
{
	//创建发送队列
	QUEUE_PacketCreate(&usart3TxQueue, usart3TxQueueBuffer, Usart3TxQueueLen);
	
	//创建接收队列
	QUEUE_PacketCreate(&usart3RxQueue, usart3RxQueueBuffer, Usart3RxQueueLen);
}

/***************************************************************************************************
*FunctionName：USART3_IRQHandler
*Description：串口3的中断函数
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:29:13
***************************************************************************************************/
void USART3_IRQHandler(void)
{			
	unsigned char cChar;

	if(USART_GetITStatus(USART3 , USART_IT_TXE) == SET)
	{

		if(QUEUE_PacketOut(&usart3TxQueue, &cChar, 1) > 0)
		{
			/* A character was retrieved from the queue so can be sent to the THR now. */
			USART_SendData(USART3, cChar);
		}
		else
		{
			/* Queue empty, nothing to send so turn off the Tx interrupt. */
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
		}
	}

	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		/* The interrupt was caused by the receiver getting data. */
		cChar = USART_ReceiveData(USART3);
		
		QUEUE_PacketIn(&usart3RxQueue, &cChar, 1);
	}
}

/***************************************************************************************************
*FunctionName：EnableUsart6TXInterrupt
*Description：开启一次发送中断（发送队列中数据）
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:18:28
***************************************************************************************************/
void EnableUsart3TXInterrupt(void)
{
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
}

MyRes SendDataByUsart3(void * data, unsigned short len)
{
	QUEUE_PacketIn(&usart3TxQueue, data, len);
	EnableUsart3TXInterrupt();
	
	return My_Pass;
}

unsigned short RecvDataByUsart3(void * data, unsigned short len)
{
	return QUEUE_PacketOut(&usart3RxQueue, data, len);
}

int fputc(int ch, FILE *f)
{
	// Place your implementation of fputc here 
	// e.g. write a character to the USART 
	USART_SendData(USART2, (uint8_t) ch);

	// Loop until the end of transmission 
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{;}

	return ch;
}

