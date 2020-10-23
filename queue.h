/******************************************************************************
 *
 * File Name: queue.h
 *
 * Description: Header file for the queues API functions
 *
 * Engineer: Youssef Taha
 *
 *******************************************************************************/
 
#include"AntonOs_Kernel.h"


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum Queue_status
{
	
    EMPTY,FULL
	
}Queue_statusType;

typedef struct queue{
	
	Queue_statusType status;
	
	uint32_t data;
	
}QueueType_t;

typedef QueueType_t QueueHandle_t;

extern tcbType *currentPtr;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void QueueCreate(QueueType_t*handle);
void QueueReceive(QueueType_t*handle,uint32_t*data,uint32_t TicksToWait);
void QueueSend (QueueType_t*handle,uint32_t*data,uint32_t TicksToWait);