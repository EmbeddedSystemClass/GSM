/***************************************************************************************************
*FileName:
*Description:
*Author:
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"HttpClient_Fun.h"
#include	"SIM800_Fun.h"
#include 	"Usart1_Driver.h"

#include	"MyMem.h"
#include	"System_Data.h"
#include	"Delay.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static char sendBuf[1024];
static char recvBuf[1024];
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static MyRes UpLoadFunction(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
void UserMainFunction(void)
{
	unsigned int i=0;
	MyRes status = My_Fail;
	
	while(i < ErrorCnt && status == My_Fail)
	{
		status = UpLoadFunction();
		
		i++;
		
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

static MyRes UpLoadFunction(void)
{
	char * tempBuf2 = NULL;

    if(My_Fail == ComWithSim800c("ATE0\r", "OK", recvBuf, 100, 3, 100 / portTICK_RATE_MS))
		return My_Fail;

	//("���ģ������ע����Ϣ\r");
	if(My_Fail == ComWithSim800c("AT+CREG?\r", "0,1", recvBuf, 100, 5, 1000 / portTICK_RATE_MS))
		return My_Fail;

	//("��ʼ��ȡsim��ICCID\r");
	if(My_Pass == ComWithSim800c("AT+CCID\r", AT_OK, recvBuf, 500, 3, 1000 / portTICK_RATE_MS))
	{
		tempBuf2 = strtok(recvBuf, "\n");
		tempBuf2 = strtok(NULL, "\n");
		setGB_Iccid(tempBuf2);
	}
	else
		return My_Fail;

	//("�ر��ƶ�����\r");
	if(My_Fail == ComWithSim800c("AT+CIPSHUT\r", "SHUT OK", recvBuf, 100, 3, 1000 / portTICK_RATE_MS))
		return My_Fail;
		
	//("����GPRS�ƶ�̨����ΪB\r");
	if(My_Fail == ComWithSim800c("AT+CGCLASS=\"B\"\r", "OK", recvBuf, 100, 1, 1000 / portTICK_RATE_MS))
		return My_Fail;
		
	//("����PDP�����ģ�1, IP, CMNET\r");
	if(My_Fail == ComWithSim800c("AT+CGDCONT=1,\"IP\",\"CMNET\"\r", "OK", recvBuf, 100, 1, 1000) / portTICK_RATE_MS)
		return My_Fail;
		
	//("����GPRSҵ��\r");
	if(My_Fail == ComWithSim800c("AT+CGATT=1\r", "OK", recvBuf, 100, 1, 1000 / portTICK_RATE_MS))
		return My_Fail;
		
	//"����GPRS����ģʽΪ: GPRS\r");
	if(My_Fail == ComWithSim800c("AT+CIPCSGP=1,\"CMNET\"\r", "OK", recvBuf, 100, 1, 1000 / portTICK_RATE_MS))
		return My_Fail;
		
	//"����Ϊ��IP����ģʽ\r");
	if(My_Fail == ComWithSim800c("AT+CIPMUX=0\r", "OK", recvBuf, 100, 1, 1000 / portTICK_RATE_MS))
		return My_Fail;
		
	//"���ý������ݲ���ʾIPͷ\r");
	if(My_Fail == ComWithSim800c("AT+CIPHEAD=0\r", "OK", recvBuf, 100, 1, 1000 / portTICK_RATE_MS))
		return My_Fail;
	
	//"��������\r");
	if(My_Fail == ComWithSim800c("AT+CIPSTART=\"TCP\",\"iot.osapling.com\",80\r", "CONNECT", recvBuf, 100, 3, 3000 / portTICK_RATE_MS))
		return My_Fail;
	
	//"������������\r");
	if(My_Fail == ComWithSim800c("AT+CIPSEND\r", ">", recvBuf, 100, 3, 2000 / portTICK_RATE_MS))
		return My_Fail;
	
	//("��������\r");
	snprintf(sendBuf, 1000, "GET /iot/datacollection.do?iccid=%s&battery=%f HTTP/1.1\nHost: iot.osapling.com\nUser-Agent: xsx\nAccept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\nConnection: keep-alive\n\n", getGB_Iccid(), getGB_BatteryV());
	ComWithSim800c(sendBuf, NULL, NULL, 1000, 1, 1000 / portTICK_RATE_MS);	

	//("\r\r���Ͳ�������Ӧ\r");
	sendBuf[0] = 0X1A;
	sendBuf[1] = 0;
	memset(recvBuf, 0, 1024);
	ComWithSim800c(sendBuf, NULL, recvBuf, 1000, 1, 3000 / portTICK_RATE_MS);	

	if(strstr(recvBuf, "1\r\n1\r\n0\r\n"))
		return My_Pass;
	else if(strstr(recvBuf, "1\r\n0\r\n0\r\n"))
		return My_Fail;
	else
		return My_Fail;
}
