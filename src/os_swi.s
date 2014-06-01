/** 

\file os_swi.s
 *
 * Author: Karl L. Waters
 * Copyright (C) 2008 by Karl L. Waters
 * 
 * History:
 * 08 JUN 2008 : Created KLW
 * 
 */


.text
.code 32
.align 0


.extern processSWI


.global callSWI
.global handleSWI



/* uint32 callSWI(void *arg1, void *arg2, void *arg3, void *arg4) */
callSWI:
	swi 8080
	
/* uint32 handleSWI(void *arg1, void *arg2, void *arg3, void *arg4) */
/* eventually save state, re-enable interrupts, and handle different swi numbers */	
handleSWI:
	stmfd 	sp!, {lr}
	bl 		processSWI
	ldmfd	sp!, {pc}


