/***************************************************************************************************
*FileName:main
*Description:程序入口
*Author:xsx
*Data:2016年4月21日14:54:04
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"SystemInit.h"

#include	"SystemStart_Task.h"

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
*FunctionName：main
*Description：程序入口
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月21日14:54:56
***************************************************************************************************/
int main(void)
{
	MySystemBSPInit();

	StartSystemStartTask();
}

/****************************************end of file************************************************/
