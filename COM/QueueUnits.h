#ifndef _QUEUE_U_H__
#define _QUEUE_U_H__

#include 	"FreeRTOS.h"
#include 	"queue.h"
#include	"semphr.h"
#include	"Define.h"

MyRes ReceiveDataFromQueue(xQueueHandle queue, void *receivedstr , unsigned short len ,
	unsigned short * readSize, unsigned short itemsize, portTickType queueBlockTime);
MyRes SendDataToQueue(xQueueHandle queue, void *sendstr , unsigned short len,  
	unsigned short itemsize, portTickType queueBlockTime, void (*fun)(void));

#endif

