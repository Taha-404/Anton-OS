/******************************************************************************
 *
 * File Name: AntonOS_kernel.h
 *
 * Description: Header file for the kernel API function
 *
 * Engineer: Youssef Taha
 *
 *******************************************************************************/

#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H
#include "AntonOS_KernelConfig.h"
#include "TM4C123.h"                    
#include <stdint.h>

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

#define SYSPRI3         (*((volatile uint32_t *)0xE000ED20))
#define INTCTRL         (*((volatile uint32_t *)0xE000ED04))
	
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum status
{
	
    RUNNING,READY,BLOCKED,DELAYED
	
}statusType;


typedef struct tcb{
	int32_t *stackPtr;        
	uint8_t piriority;
	statusType status;
	uint32_t delayTime;
}tcbType;

typedef uint32_t TaskHandle_t ;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/


void OS_KernelLaunch(void);
>>>>>>> 0711a2e910b18ac135f1315ac3370babdf9003dc
void OS_KernelInit(void);void OS_SchedulerLaunch(void);
void checkPiriority(void);
void getMaxPiriorityTask (void);
void checkDelayedTasks(void);
void taskYield(void);
void TaskCreate(void(*task)(void),uint8_t piriority ,TaskHandle_t*handle);
void TaskBlock(TaskHandle_t handle);
void TaskResume(TaskHandle_t handle);
void TaskDelay(uint32_t ticks ,TaskHandle_t handle);



#endif
													 

