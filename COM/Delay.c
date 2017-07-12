/***************************************************************************************************
*FileName:Delay
*Description:延时功能
*Author:xsx
*Data:2016年4月21日14:22:46
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"Delay.h"

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
*FunctionName：delay_us
*Description：微妙延时
*Input：nus -- 延时时间
*Output：None
*Author：xsx
*Data：2016年4月21日14:23:05
***************************************************************************************************/
void delay_us(unsigned short nus)
{
	unsigned short i=0,j=0;
	
	for(i=0; i<nus; i++)
		for(j=0;j<6;j++);
}

/***************************************************************************************************
*FunctionName：delay_ms
*Description：毫秒延时
*Input：nms -- 延时时间
*Output：None
*Author：xsx
*Data：2016年4月21日14:25:55
***************************************************************************************************/
void delay_ms(unsigned short nms)
{
	unsigned short i=0; 

	for(i=0; i<nms; i++)
        delay_us(1000);
}

/***************************************************************************************************
*FunctionName：delay_s
*Description：秒延时
*Input：ns -- 延时时间
*Output：None
*Author：xsx
*Data：2016年4月21日14:25:55
***************************************************************************************************/
void delay_s(unsigned short ns)
{
	unsigned short i=0;  
	for(i=0; i<ns; i++)
        delay_ms(1000);
}

