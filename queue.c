/**********************************************************************************
 *
 * File Name: queue.c
 *
 * Description: Source file for the queues API functions
 *
 * Engineer: Youssef Taha
 *
***********************************************************************************/
#include"queue.h"

/**********************************************************************************
*                       Functions Definitions                                     *
***********************************************************************************/

/***********************************************************************************
* [Function Name]: QueueCreate
*
* [Description]:   creating a queue
*
* [Arguments]:	   handle of the queue
*
* [Retuen]:		   	 void
***********************************************************************************/


void QueueCreate(QueueType_t*handle)
{
	/*switchine the state of the queue to empty state*/
	handle->status =EMPTY;
}

/***********************************************************************************
* [Function Name]: QueueReceive
*
* [Description]:   receiving data from a queue
*
* [Arguments]:	   handle of the queue , address to store the data, blocking time of the task   
*
* [Retuen]:		   	 void
***********************************************************************************/

void QueueReceive(QueueType_t*handle,uint32_t*data,uint32_t TicksToWait)
{
	if(handle->status==FULL)
	{
		/*storing the data of the queue in the given address*/
		*data=handle->data;
		/*switching the state of the queue to empty state*/
		handle->status=EMPTY;
	}
	else
	{
		/*delay the task with the given time */
		TaskDelay(TicksToWait,(uint32_t)currentPtr);
	}
}

/***********************************************************************************
* [Function Name]: QueueSend
*
* [Description]:   sending data to a queue
*
* [Arguments]:	   handle of the queue , address of the data, blocking time of the task   
*
* [Retuen]:		   	 void
***********************************************************************************/

void QueueSend (QueueType_t*handle,uint32_t*data,uint32_t TicksToWait)
{
		if(handle->status==EMPTY)
	{
		/*storing the given data in the queue*/
		handle->data=*data;
		/*switching the state of the queue to the full state*/
		handle->status=FULL;
	}
	else
	{
		/*delay the task with the given time */
		TaskDelay(TicksToWait,(uint32_t)currentPtr);
	}
}