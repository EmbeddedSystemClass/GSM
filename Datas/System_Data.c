/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List**********************************************/
/***************************************************************************************************/
#include	"System_Data.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

static char gb_ICCID[21];                                                       //SIM卡的iccid，20位

static float gb_BatteryV = 0;											        //电池电压

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/


/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/***************************************************************************************************
*FunctionName: setGB_Iccid
*Description: 更新iccid数据
*Input: iccid -- iccid输入域
*Output: 
*Return: 
*Author: xsx
*Date: 2017-6-26 21:16:01
***************************************************************************************************/
void setGB_Iccid(const char * iccid)
{
    if(iccid)
        snprintf(gb_ICCID, 21, "%.20s", iccid);
    else
        memset(gb_ICCID, 0, 21);
}

/***************************************************************************************************
*FunctionName: getGB_Iccid
*Description: 获取iccid数据
*Input: 
*Output: 
*Return: iccid保存域指针
*Author: xsx
*Date: 
***************************************************************************************************/
const char * getGB_Iccid(void)
{
    return gb_ICCID;
}

/***************************************************************************************************
*FunctionName: setGB_BatteryV
*Description: 更新电池电压数据
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年6月26日21:17:46
***************************************************************************************************/
void setGB_BatteryV(float value)
{
    gb_BatteryV = value;
}

/***************************************************************************************************
*FunctionName: getGB_BatteryV
*Description: 获取电池电压数据
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2017年6月26日21:17:59
***************************************************************************************************/
float getGB_BatteryV(void)
{
    return gb_BatteryV;
}

/****************************************end of file************************************************/
