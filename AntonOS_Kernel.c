/******************************************************************************
 *
 * File Name: OsKernel.c
 *
 * Description: Source file for the kernel API functions
 *
 * Engineer: Youssef Taha
 *
 *******************************************************************************/

#include "AntonOS_Kernel.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

uint32_t millsPrescaler;
static volatile uint32_t OS_Ticks=0;
tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPtr;
int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/***********************************************************************************
* [Function Name]: OS_KernelStackInit
*
* [Description]:   initialize the stack pointer for the threads
*
* [Arguments]:	   Index of the thread
*
* [Retuen]:		   void
***********************************************************************************/

void OS_KernelStackInit(uint32_t i)
{
	/*stack pinter of the thread pointing to the [STACKSIZE-16] address in the thread stack */
  tcbs[i].stackPtr = &TCB_STACK[i][STACKSIZE-16]; 
  	/* declaring we are in thumb mode*/
  TCB_STACK[i][STACKSIZE-1] = 0x01000000;  
}

/***********************************************************************************
* [Function Name]: OS_KernelInit
*
* [Description]:   initializing the millsPrescaller
*
* [Arguments]:	   void
*
* [Retuen]:		   void
***********************************************************************************/

void OS_KernelInit(void)
{
     __disable_irq();
		 millsPrescaler=(F_CPU/1000);
	 __enable_irq();

}
/***********************************************************************************
* [Function Name]: OS_KernelLaunch
*
* [Description]:   Launching the kernel
*
* [Arguments]:	   quanta in milliseconds
*
* [Retuen]:		   void
***********************************************************************************/

void OS_KernelLaunch(uint32_t quanta)
{
	/*initialize the millsPrescaler*/
	OS_KernelInit();				
	/*get the address of the highest piriority task in the current pointer*/										
	getMaxPiriorityTask ();		
	/*changing the state of the current task to running*/								    
	currentPtr->status = RUNNING;		
	/*disable the systick timer*/									
	SysTick->CTRL =0;			
	/*putting zero in the value register of the systick timer*/											    
	SysTick->VAL=0;			
	/*putting quanta in the load register of the systick timer*/													
	SysTick->LOAD = (quanta* millsPrescaler)-1;			
	/*making the piriority of the systick timer interrupt the lowest piriority possible*/						
  	SYSPRI3 =(SYSPRI3&0x00FFFFFF)|0xE0000000;		
  	/*lauunching the systick timer*/								
	SysTick->CTRL =0x00000007;	
	/*Launching the scheduler*/											
	OS_SchedulerLaunch();														
}

/***********************************************************************************
* [Function Name]: checkPiriority
*
* [Description]:   Launching the highest piriority thread
*
* [Arguments]:	   quanta in milliseconds
*
* [Retuen]:		   void
***********************************************************************************/

void checkPiriority(void)
{
	/*putting the crunning task in the ready state*/
	uint8_t i ;

	for(i=0;i<NUM_OF_THREADS;i++)
	{
		if (tcbs[i].status == RUNNING) 
		{
			tcbs[i].status= READY;
		}
	}
			/*checking if any of the delayed tasks served its delay time*/
	    	checkDelayedTasks();	
			/*putting the highest piriority task address in the current pointer*/										    				
			getMaxPiriorityTask();	
			/*switching the state of the current thread to running*/											    
			currentPtr->status = RUNNING;		
			 /*increment the OS_Ticks*/								     
			OS_Ticks++;															   
}

/***********************************************************************************
* [Function Name]: TaskCreate
*
* [Description]:   Creating the threads
*
* [Arguments]:	   Task address , Task piriority , Task handle
*
* [Retuen]:		     void
***********************************************************************************/

void TaskCreate(void(*task)(void),uint8_t piriority , TaskHandle_t*handle)
{
	static uint8_t i =0;
	__disable_irq();
	/*initialize the stack of the thread*/
	OS_KernelStackInit(i);	
	/*assign the thread piriority*/													    
	tcbs[i].piriority=piriority;
	/*the default state of the tasks is the ready state*/											    
	tcbs[i].status=READY;		
	/*the default delay time of the task is 0*/												    
	tcbs[i].delayTime=0;	
	/*stack pointer of the task = address of the thread*/												    
	TCB_STACK[i][STACKSIZE-2] = (int32_t)(task); 		
	/*the handle of the thread = the stack pointer*/						    
	*handle=(uint32_t)&tcbs[i];													    
	i++;
	__enable_irq();  
}

/***********************************************************************************
* [Function Name]: getMaxPiriorityTask
*
* [Description]:   Putting max piriorty task address in current pointer
*
* [Arguments]:	   void
*
* [Retuen]:		     void
***********************************************************************************/

void getMaxPiriorityTask (void)
{
	uint16_t max=0;
	uint8_t i ;
	/*check for the highest piriority task in the ready state and put its address in the current pointer*/
	for(i=0;i<NUM_OF_THREADS;i++)
	{
		if (tcbs[i].piriority>max && tcbs[i].status==READY )
		{
			/*updating the max value*/
			max=tcbs[i].piriority;
			/*putting the address of the thread in the current pointer*/
			currentPtr=&tcbs[i];
		}
	}
}

/***********************************************************************************
* [Function Name]: TaskBlock
*
* [Description]:   Putting the task in the blocking state
*
* [Arguments]:	   task handle 
*
* [Retuen]:		     void
***********************************************************************************/

void TaskBlock(TaskHandle_t handle)
{
	
	tcbType*taskHandler=(tcbType*)handle;
	/*swicthing the task state to the blocked state*/
	taskHandler->status =BLOCKED;	
	/*ask for context switch*/												
	taskYield();																	
}

/***********************************************************************************
* [Function Name]: TaskResume
*
* [Description]:   Putting the task in the ready state
*
* [Arguments]:	   task handle 
*
* [Retuen]:		     void
***********************************************************************************/

void TaskResume(TaskHandle_t handle)
{
	tcbType*taskHandler=(tcbType*)handle;
	/*swicthing the task state to the ready state*/
	taskHandler->status =READY;		
	/*ask for context switch*/											   
	taskYield();																   
}

/***********************************************************************************
* [Function Name]: TaskDelay
*
* [Description]:   Putting the task in the delayes state for certain time
*
* [Arguments]:	   task handle , delay time in quanta
*
* [Retuen]:		     void
***********************************************************************************/

void TaskDelay(uint32_t ticks,TaskHandle_t handle)
{
	tcbType*taskHandler=(tcbType*)handle;
	/*if the delayed time equal zero retun to the task*/
	if(ticks==0)
	{
		return;
	}
	/*updating the dalay time of the thread with the delayed time+the value of the OS_Ticks*/
	taskHandler->delayTime = ticks+OS_Ticks;
	/*switching the state of the task to delayed state*/
	taskHandler->status = DELAYED;
	/*ask for context switch*/
	taskYield();
}	
/***********************************************************************************
* [Function Name]: checkDelayedTasks
*
* [Description]:   checking if any of the delayed tasks served its delay time
*
* [Arguments]:	   void
*
* [Retuen]:		     void
***********************************************************************************/

void checkDelayedTasks(void)
{
	uint8_t i;
	for(i=0;i<NUM_OF_THREADS;i++)
	{
		/*checking if any of the delayed tasks served its delay time*/
		if(tcbs[i].status ==DELAYED && tcbs[i].delayTime == OS_Ticks)
		{
			/*swiching the state of the thread to ready state*/
			tcbs[i].status=READY;
			/*update the delay time of the task*/
			tcbs[i].delayTime=0;
		}
	}
}

/***********************************************************************************
* [Function Name]: taskYield
*
* [Description]:   force the systick handler interrupt to do a context switch
*
* [Arguments]:	   void
*
* [Retuen]:		     void
***********************************************************************************/

 void taskYield(void)
{
	/*putting zero in the value register of the systick timer*/
	SysTick->VAL=0;
	/*putting this value in INTCTRL register fires systick timer interrupt */
	INTCTRL = 0x04000000;
}






