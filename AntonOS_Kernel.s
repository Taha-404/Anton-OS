		AREA |.text|, CODE, READONLY , ALIGN=4
		PRESERVE8
        THUMB
		EXTERN  currentPtr
		EXPORT	SysTick_Handler
		EXPORT  OS_SchedulerLaunch
		IMPORT	checkPiriority


SysTick_Handler          ;save r0,r1,r2,r3,r12,lr,pc,psr    
    CPSID   I               ; disabling the interrupts            
    PUSH    {R4-R11}        ; save r4,r5,r6,r7,r8,r9,r10,r11   
    LDR     R0, =currentPtr     ; r0 points to currentPt
    LDR     R1, [R0]        ; r1= currentPt   
    STR     SP, [R1]        ; these four lines above save the context 
    PUSH	{R0,LR}
	BL      checkPiriority
	POP		{R0,LR} 
	LDR		R1,[R0]          ;R1= currentPt i.e. New thread
    LDR     SP, [R1]        ; SP= currentPt->stackPt   
    POP     {R4-R11}        ; loading the new context from the saved stack frame   
    CPSIE   I                  
    BX      LR 
	
	

OS_SchedulerLaunch
	CPSID   I 
    LDR     R0, =currentPtr         
    LDR     R2, [R0]       ; R2 =currentPt       
    LDR     SP, [R2]       ;SP = currentPt->stackPt    
    POP     {R4-R11}          
    POP     {R0-R3}            
    POP     {R12}
    ADD     SP,SP,#4           
    POP     {LR}               
    ADD     SP,SP,#4         
    CPSIE   I                 
    BX      LR  
	
	ALIGN
    END