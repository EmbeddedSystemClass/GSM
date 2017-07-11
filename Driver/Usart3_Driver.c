/***************************************************************************************************
*FileName:Usart3_Driver
*Description:����3����
*Author:xsx
*Data:2016��4��29��11:50:01
***************************************************************************************************/






/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include 	"stm32l1xx.h"
#include 	"Usart3_Driver.h"

#include "MyQueue.h"
#include 	"stdio.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static QUEUE8_t usart3TxQueue;								//����2���Ͷ���
static unsigned char usart3TxQueueBuffer[Usart3TxQueueLen];	//����2���Ͷ���������

static QUEUE8_t usart3RxQueue;								//����2���ն���
static unsigned char usart3RxQueueBuffer[Usart3RxQueueLen];	//����2���ն���������
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static void usart3QueueInit(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��ConfigUsart3
*Description������3�Ķ˿ڳ�ʼ��������
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��29��11:52:08
***************************************************************************************************/
void Usart3_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* ����GPIO_D��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* ��������3��ʱ�� */
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

	/* ʹ�ܴ���2 */
	USART_Cmd(USART3, ENABLE);
	//ʹ�ܽ����ж�
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
	//�������Ͷ���
	QUEUE_PacketCreate(&usart3TxQueue, usart3TxQueueBuffer, Usart3TxQueueLen);
	
	//�������ն���
	QUEUE_PacketCreate(&usart3RxQueue, usart3RxQueueBuffer, Usart3RxQueueLen);
}

/***************************************************************************************************
*FunctionName��USART3_IRQHandler
*Description������3���жϺ���
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��29��11:29:13
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
*FunctionName��EnableUsart6TXInterrupt
*Description������һ�η����жϣ����Ͷ��������ݣ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��29��11:18:28
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

