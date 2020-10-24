
#include "AntonOS_Kernel.h"
#include"semaphore.h"

uint32_t count0,count1,count2;
TaskHandle_t handle0;
TaskHandle_t handle1;
TaskHandle_t handle2;

SemaphoreHandle_t semaphore0;
SemaphoreHandle_t semaphore1;

void Task0(void)
{
			SemaphoreTake(&semaphore0,10);
	while(1)
	{
		count0++;
		TaskBlock(handle0);
	}
	
}


void Task1(void)
{
			SemaphoreTake(&semaphore0,5);
	while(1)
	{
		count1++;
		SemaphoreTake(&semaphore0,5);
	}
	
}


void Task2(void)
{
			SemaphoreGive(&semaphore0);
	while(1)
	{
		count2++;
	 SemaphoreGive(&semaphore0);

		
	}
	
}


int main(void)
{
	SemaphoreCreateBinary(&semaphore0);
	TaskCreate(Task0,3,&handle0);
	TaskCreate(Task1,2,&handle1);
	TaskCreate(Task2,1,&handle2);
	OS_KernelLaunch(QUANTA);
}

