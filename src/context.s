

.global TimerTickISR
.global Restore_Context
.global TestISR

.text
.code 32
.align 0
     

TimerTickISR:
	/* -- Save_Context -- */ 
/* Adapted from FreeRTOS V5.0.0 port to lpc23xx for GCC*/
/* cannot be in sys or usr modes */
	/* Correct for LR offset in irq mode */ 
	SUB	LR, LR, #4
	/*store r0 so it can be used to get the usr/sys stack pointer */
	STMDB	SP!, {R0}

	/* Set R0 to point to the task (usr/sys) stack pointer. */
	STMDB	SP,{SP}^
	NOP
	SUB	SP, SP, #4
	LDMIA	SP!,{R0}

	/* Push the return address onto the stack. */
	STMDB	R0!, {LR}

	/* LR_irq is where the task should resume, so it should become PC_usr */
	/* Now that it is saved to the task stack we can use the LR_irq register
	 * to store the task stack pointer instead of R0. */
	MOV	LR, R0

	/* Pop R0 so we can save it onto the system mode stack. */
	LDMIA	SP!, {R0}

	/* All the task registers (usr/sys) are now in the state they were before the interrupt
	 * and the task stack pointer is stored at LR_irq */
	/* Push all the system mode registers onto the task stack. */
	STMDB	LR,{R0-LR}^
	NOP
	SUB	LR, LR, #60

	/* Push the SPSR onto the task stack. */
	MRS	R0, SPSR
	STMDB	LR!, {R0}

/*
	LDR	R0, =ulCriticalNesting
	LDR	R0, [R0]
	STMDB	LR!, {R0}
*/

	/* Store the new top of stack for the task. */
	LDR	R0, =kos_threadCurr
	LDR	R0, [R0]
	STR	LR, [R0]
	
	/* -- End Save Context -- */
	
	LDR		r2, =kos_TimerTick
	MOV		lr, pc
	BX		r2			/* jump to kos_TimerTick */
	
	LDR		r2, =kos_ScheduleNext
	MOV		lr, pc
	BX		r2			/* jump to kos_ScheduleNext */
	
	
	/* return and continue with Restore_Context */
	

/* Save_Context - Adapted from FreeRTOS V5.0.0 port to lpc23xx for GCC*/
/* cannot be in sys or usr modes */
/* at this point, either don't care about the current context or it has been already saved. */
Restore_Context:
	/* Get the pointer to stack of the task to be restored */
	LDR		R0, =kos_threadCurr
	LDR		R0, [R0]
	LDR		LR, [R0]
	
	/*
	LDR		R0, =ulCriticalNesting
	LDMFD	LR!, {R1}
	STR		R1, [R0]
	*/
	
	/* Get the SPSR from the stack. */
	LDMFD	LR!, {R0}
	MSR		SPSR, R0
	
	/* Restore all system mode registers for the task. */
	LDMFD	LR, {R0-R14}^
	NOP
	
	/* Restore the return address. */
	/* Just pulling the next item off the stack - 15 registers * 4 bytes = 60 bytes */
	LDR		LR, [LR, #+60]
	
	/* And return */
	MOVS	PC, LR
	
	
	
	
TestISR:
	SUB		lr, lr, #4
	STMFD	sp!, {r0-r3, r12, lr}		/* r12 frame pointer - might be used */
	
	LDR		r2, =kos_TimerTick
	MOV		lr, pc
	BX		r2			/* jump to kos_TimerTick */
	
	LDMFD	sp!, {r0-r3, r12, pc}^

.end
