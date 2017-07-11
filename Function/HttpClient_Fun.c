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
#include 	"Usart3_Driver.h"
#include	"SimPwrKey_Driver.h"

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
static char recvBuf[4096];
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

unsigned char UpLoadFunction(unsigned short cnt)
{
	char * tempBuf2 = NULL;

	printf("gprsģ���ʼ��...\r\r");
		
	//���ģ���Ƿ���������
	printf("����gprsģ������״̬...\r");
    if(My_Pass != ComWithSim800c("ATE0\r", "OK", recvBuf, 100, 10, 500))
	{
		printf("ģ������ʧ��\r\r");
		return 0;
	}
	else
	{
		printf("ģ�����ӳɹ�\r\r");
	}

	//CREG?
	printf("���ģ������ע����Ϣ\r");
	if(My_Fail == ComWithSim800c("AT+CREG?\r", "0,1", recvBuf, 100, 10, 500))
	{
		printf("ģ������ע��ʧ��\r\r");
		return 0;
	}
	else
	{
		printf("ģ������ע��ɹ�\r\r");
	}


	printf("��ʼ��ȡsim��ICCID\r");
	if(My_Pass == ComWithSim800c("AT+CCID\r", AT_OK, recvBuf, 500, 3, 1000))
	{
		tempBuf2 = strtok(recvBuf, "\n");
		tempBuf2 = strtok(NULL, "\n");
		setGB_Iccid(tempBuf2);
		
		printf("�ɹ���ȡICCID:%s\r\r", getGB_Iccid());
	}
	else
	{
		printf("��ȡICCIDʧ��\r\r");
		return 0;
	}
		
	printf("�ر��ƶ�����\r");
	if(My_Pass == ComWithSim800c("AT+CIPSHUT\r", "SHUT OK", recvBuf, 100, 3, 1000))
		printf("�ر��ƶ������ɹ�\r\r");
	else
	{
		printf("�ر��ƶ�����ʧ��\r\n");
		return 0;
	}
		
	printf("����GPRS�ƶ�̨����ΪB\r");
	if(My_Pass == ComWithSim800c("AT+CGCLASS=\"B\"\r", "OK", recvBuf, 100, 1, 1000))
		printf("�����ɹ�\r\r");
	else
	{
		printf("����ʧ��\r\r");
		return 0;
	}
		
	printf("����PDP�����ģ�1, IP, CMNET\r");
	if(My_Pass == ComWithSim800c("AT+CGDCONT=1,\"IP\",\"CMNET\"\r", "OK", recvBuf, 100, 1, 1000))
		printf("�����ɹ�\r\r");
	else
	{
		printf("����ʧ��\r\r");
		return 0;
	}
		
	printf("����GPRSҵ��\r");
	if(My_Pass == ComWithSim800c("AT+CGATT=1\r", "OK", recvBuf, 100, 1, 1000))
		printf("�����ɹ�\r\r");
	else
	{
		printf("����ʧ��\r\r");
		return 0;
	}
		
	printf("����GPRS����ģʽΪ: GPRS\r");
	if(My_Pass == ComWithSim800c("AT+CIPCSGP=1,\"CMNET\"\r", "OK", recvBuf, 100, 1, 1000))
		printf("�����ɹ�\r\r");
	else
	{
		printf("����ʧ��\r\r");
		return 0;
	}
		
	printf("����Ϊ��IP����ģʽ\r");
	if(My_Pass == ComWithSim800c("AT+CIPMUX=0\r", "OK", recvBuf, 100, 1, 1000))
		printf("�����ɹ�\r\r");
	else
	{
		printf("����ʧ��\r\r");
		return 0;
	}
		
	printf("���ý������ݲ���ʾIPͷ\r");
	if(My_Pass == ComWithSim800c("AT+CIPHEAD=0\r", "OK", recvBuf, 100, 1, 1000))
		printf("�����ɹ�\r\r");
	else
	{
		printf("����ʧ��\r\r");
		return 0;
	}
	
	printf("��������\r");
	if(My_Pass == ComWithSim800c("AT+CIPSTART=\"TCP\",\"iot.osapling.com\",80\r", "CONNECT", recvBuf, 100, 3, 2000))
	{
		printf("���ӳɹ�\r\r");
	}
	else
	{
		printf("����ʧ��\r\r");
		return 0;
	}
	
	printf("������������\r");
	if(My_Pass == ComWithSim800c("AT+CIPSEND\r", ">", recvBuf, 100, 3, 2000))
	{
		printf("���ӳɹ�\r\r");
	}
	else
	{
		printf("����ʧ��\r\r");
		return 0;
	}
	
	printf("��������\r");
	snprintf(sendBuf, 1000, "GET /iot/datacollection.do?iccid=%s&battery=%d HTTP/1.1\nHost: iot.osapling.com\nUser-Agent: xsx\nAccept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\nConnection: keep-alive\n\n", getGB_Iccid(), cnt);
	ComWithSim800c(sendBuf, NULL, NULL, 1000, 1, 1000);	

	printf("\r\r���Ͳ�������Ӧ\r");
	sendBuf[0] = 0X1A;
	sendBuf[1] = 0;
	memset(recvBuf, 0, 4096);
	ComWithSim800c(sendBuf, NULL, recvBuf, 1000, 1, 3000);	

	printf(recvBuf, strlen(recvBuf));
	
	if(strstr(recvBuf, "1\r\n1\r\n0\r\n"))
		return 1;
	else if(strstr(recvBuf, "1\r\n0\r\n0\r\n"))
		return 2;
	else
	{
		printf("�����ύ�ɹ�\r");
		return 0;
	}
}
