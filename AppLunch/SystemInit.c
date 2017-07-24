/***************************************************************************************************
*FileName:SystemInit
*Description: 系统所有模块初始化
*Author : xsx
*Data: 2016年4月21日14:18:28
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"SystemInit.h"

#include	"Led_Driver.h"
#include 	"Usart1_Driver.h"
#include	"Pwr_Driver.h"
#include	"BettryADC.h"

#include	"Delay.h"
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
extern void SystemInit(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName：MySystemInit
*Description：系统初始化
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月21日14:20:44
***************************************************************************************************/
void MySystemBSPInit(void)
{
	delay_ms(100);
	
//	Pwr_GPIOInit();						//系统指示灯初始化
	delay_ms(10);
	
//	BettryADC_Init();
	delay_ms(10);
	
	Led_GPIOInit();						//系统指示灯初始化
	delay_ms(10);

//	Usart1_Init();						//串口3初始化
	delay_ms(10);
}

