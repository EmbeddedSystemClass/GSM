/***************************************************************************************************
*FileName:Usart2_Driver
*Description:����2����
*Author:xsx
*Data:2016��4��29��11:57:28
***************************************************************************************************/


/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include "stm32l1xx.h"
#include "Usart1_Driver.h"
#include "MyQueue.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static QUEUE8_t usart1TxQueue;								//����2���Ͷ���
static unsigned char usart1TxQueueBuffer[Usart1TxQueueLen];	//����2���Ͷ���������

static QUEUE8_t usart1RxQueue;								//����2���ն���
static unsigned char usart1RxQueueBuffer[Usart1RxQueueLen];	//����2���ն���������
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��ConfigUsart2
*Description������2�Ķ˿ڳ�ʼ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��29��11:58:48
***************************************************************************************************/
void Usart1_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* ����GPIO_D��ʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);
	/* ��������3��ʱ�� */
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

	/* ʹ�ܴ���2 */
	USART_Cmd(USART1, ENABLE);
	//ʹ�ܽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	/* NVIC configuration */
	/* Configure the Priority Group to 2 bits */


	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//�������Ͷ���
	QUEUE_PacketCreate(&usart1TxQueue, usart1TxQueueBuffer, Usart1TxQueueLen);
	
	//�������ն���
	QUEUE_PacketCreate(&usart1RxQueue, usart1RxQueueBuffer, Usart1RxQueueLen);
}

/***************************************************************************************************
*FunctionName��USART2_IRQHandler
*Description������2�жϺ���
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��29��11:59:24
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
*FunctionName��EnableUsart2TXInterrupt
*Description������һ�η����жϣ����Ͷ��������ݣ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��29��11:18:28
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

