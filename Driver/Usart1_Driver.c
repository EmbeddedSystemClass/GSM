/***************************************************************************************************
*FileName:Usart2_Driver
*Description:串口2驱动
*Author:xsx
*Data:2016年4月29日11:57:28
***************************************************************************************************/


/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include "stm32l1xx.h"
#include "Usart1_Driver.h"
#include "MyQueue.h"
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
static QUEUE8_t usart1TxQueue;								//串口2发送队列
static unsigned char usart1TxQueueBuffer[Usart1TxQueueLen];	//串口2发送队列数据域

static QUEUE8_t usart1RxQueue;								//串口2接收队列
static unsigned char usart1RxQueueBuffer[Usart1RxQueueLen];	//串口2接收队列数据域
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName：ConfigUsart2
*Description：串口2的端口初始化
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:58:48
***************************************************************************************************/
void Usart1_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 开启GPIO_D的时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);
	/* 开启串口3的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);


	USART_InitStructure.USART_BaudRate   = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART1, &USART_InitStructure);

	/* 使能串口2 */
	USART_Cmd(USART1, ENABLE);
	//使能接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	/* NVIC configuration */
	/* Configure the Priority Group to 2 bits */


	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//创建发送队列
	QUEUE_PacketCreate(&usart1TxQueue, usart1TxQueueBuffer, Usart1TxQueueLen);
	
	//创建接收队列
	QUEUE_PacketCreate(&usart1RxQueue, usart1RxQueueBuffer, Usart1RxQueueLen);
}

/***************************************************************************************************
*FunctionName：USART2_IRQHandler
*Description：串口2中断函数
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:59:24
***************************************************************************************************/
void USART1_IRQHandler(void)
{			
	unsigned char cChar;

	if(USART_GetITStatus(USART1 , USART_IT_TXE) == SET)
	{

		if(QUEUE_PacketOut(&usart1TxQueue, &cChar, 1) > 0)
		{
			/* A character was retrieved from the queue so can be sent to the THR now. */
			USART_SendData(USART1, cChar);
		}
		else
		{
			/* Queue empty, nothing to send so turn off the Tx interrupt. */
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}
	}

	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		/* The interrupt was caused by the receiver getting data. */
		cChar = USART_ReceiveData(USART1);
		
		QUEUE_PacketIn(&usart1RxQueue, &cChar, 1);
	}
}

/***************************************************************************************************
*FunctionName：EnableUsart2TXInterrupt
*Description：开启一次发送中断（发送队列中数据）
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:18:28
***************************************************************************************************/
void EnableUsart1TXInterrupt(void)
{
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

MyRes SendDataByUsart1(void * data, unsigned short len)
{
	QUEUE_PacketIn(&usart1TxQueue, data, len);
	EnableUsart1TXInterrupt();
	
	return My_Pass;
}

unsigned short RecvDataByUsart1(void * data, unsigned short len)
{
	return QUEUE_PacketOut(&usart1RxQueue, data, len);
}

