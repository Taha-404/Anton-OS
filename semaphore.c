/**********************************************************************************
 *
 * File Name: semaphore.c
 *
 * Description: Source file for the semaphores API functions
 *
 * Engineer: Youssef Taha
 *
***********************************************************************************/

#include"semaphore.h"

/**********************************************************************************
 *                      Functions Definitions                                  *
***********************************************************************************/

/***********************************************************************************
* [Function Name]: SemaphoreCreateBinary
*
* [Description]:   creating binary semaphore
*
* [Arguments]:	   handle of the semaphore
*
* [Return]:		   	 void
***********************************************************************************/

void SemaphoreCreateBinary( SemaphoreHandle_t * semaphore )
{

	__disable_irq();
	/*initialize the counter value with 1*/
	*semaphore=1;
	__enable_irq(); 

}

/***********************************************************************************
* [Function Name]: SemaphoreCreateCounting
*
* [Description]:   creating counting semaphore
*
* [Arguments]:	   handle of the semaphore, blocking time of the task
*
* [Return]:		   	 void
***********************************************************************************/

void SemaphoreCreateCounting( SemaphoreHandle_t * semaphore , uint32_t MaxCount)
{

	__disable_irq();
	/*initialize the counter value with MaxCount*/
	*semaphore=MaxCount;
	__enable_irq(); 

}

/***********************************************************************************
* [Function Name]: SemaphoreTake
*
* [Description]:   taking semaphore
*
* [Arguments]:	   handle of the semaphore, blocking time of the task
*
* [Return]:		   	 void
***********************************************************************************/

void SemaphoreTake( SemaphoreHandle_t * semaphore,uint32_t TicksToWait )
{
	/*if the semaphore is taken delay the thread with TicksToWait value */
	while(*semaphore==0)
	{
		TaskDelay(TicksToWait,(uint32_t)currentPtr);
	}
	__disable_irq();
	/*if the semaphore is given decrement the counter then continue*/
	*semaphore=*semaphore-1;
	__enable_irq(); 
}

/***********************************************************************************
* [Function Name]: SemaphoreGive
*
* [Description]:   giving semaphore
*
* [Arguments]:	   handle of the semaphore
*
* [Return]:		   	 void
***********************************************************************************/

void SemaphoreGive( SemaphoreHandle_t * semaphore )
{

	__disable_irq();
	/*increment the counter*/
	*semaphore=*semaphore+1;
	/*ask for context switch*/
	taskYield();
	__enable_irq(); 

}






