/******************************************************************************
 *
 * File Name: semaphore.h
 *
 * Description: Header file for the semaphores API functions
 *
 * Engineer: Youssef Taha
 *
 *******************************************************************************/
#ifndef SEMAPHORE_H_
#define	SEMAPHORE_H_


#include"AntonOs_Kernel.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef uint32_t SemaphoreHandle_t ;

extern tcbType *currentPtr;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

extern void TaskDelay(uint32_t ticks,TaskHandle_t handle);
void SemaphoreCreateBinary( SemaphoreHandle_t * semaphore );
void SemaphoreCreateCounting( SemaphoreHandle_t * semaphore , uint32_t MaxCount);
void SemaphoreTake( SemaphoreHandle_t * semaphore,uint32_t TicksToWait );
void SemaphoreGive( SemaphoreHandle_t * semaphore );
#endif
