/******************************************************************************************
*文件名：Iwdg_Task.c
*描述：独立看门狗任务
*说明：优先级1（最低），1S喂狗一次，
*作者：xsx
******************************************************************************************/


#include	"Iwdg_Task.h"
#include	"Iwdg_Driver.h"

#include 	"FreeRTOS.h"
#include 	"task.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/

#define vIwdgTask_PRIORITY			( ( unsigned portBASE_TYPE ) 1U )
const char * IwdgTaskName = "Iwdg\0";		//看门狗任务名
/******************************************************************************************/
/*****************************************局部函数声明*************************************/

static void vIwdgTask( void *pvParameters );

/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/************************************************************************
** 函数名:StartvIwdgTask
** 功  能:创建并启动喂狗任务
** 输  入:无
** 输  出:无
** 返  回：无
** 备  注：无
** 时  间: 2015年5月15日 17:04:43 
** 作  者：xsx                                                 
************************************************************************/
char StartvIwdgTask(void)
{
	return xTaskCreate( vIwdgTask, IwdgTaskName, configMINIMAL_STACK_SIZE, NULL, vIwdgTask_PRIORITY, NULL );
}

/************************************************************************
** 函数名:vIwdgTask
** 功  能:喂狗任务
** 输  入:无
** 输  出:无
** 返  回：无
** 备  注：无
** 时  间: 2015年5月15日 17:04:12  
** 作  者：xsx                                                 
************************************************************************/
static void vIwdgTask( void *pvParameters )
{

	while(1)
	{
		IWDG_Feed();
		vTaskDelay(500 * portTICK_RATE_MS);
	}
}

