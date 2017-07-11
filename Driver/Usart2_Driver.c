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
#include "Usart2_Driver.h"
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/

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
void Usart2_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 开启GPIO_D的时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	/* 开启串口3的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);


	USART_InitStructure.USART_BaudRate   = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART2, &USART_InitStructure);

	/* 使能串口2 */
	USART_Cmd(USART2, ENABLE);
	//使能接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	/* NVIC configuration */
	/* Configure the Priority Group to 2 bits */


	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/***************************************************************************************************
*FunctionName：USART2_IRQHandler
*Description：串口2中断函数
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:59:24
***************************************************************************************************/
void USART2_IRQHandler(void)
{			
	
}

/***************************************************************************************************
*FunctionName：EnableUsart2TXInterrupt
*Description：开启一次发送中断（发送队列中数据）
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日11:18:28
***************************************************************************************************/
void EnableUsart2TXInterrupt(void)
{
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
}


