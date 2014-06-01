

/*
 * Variables
 */



/*
 * Some defines for the program status registers
 */
   ARM_MODE_USER  = 0x10      /* Normal User Mode                             */ 
   ARM_MODE_FIQ   = 0x11      /* FIQ Fast Interrupts Mode                     */
   ARM_MODE_IRQ   = 0x12      /* IRQ Standard Interrupts Mode                 */
   ARM_MODE_SVC   = 0x13      /* Supervisor Interrupts Mode                   */
   ARM_MODE_ABORT = 0x17      /* Abort Processing memory Faults Mode          */
   ARM_MODE_UNDEF = 0x1B      /* Undefined Instructions Mode                  */
   ARM_MODE_SYS   = 0x1F      /* System Running in Priviledged Operating Mode */
   ARM_MODE_MASK  = 0x1F
   
   I_BIT          = 0x80      /* disable IRQ when I bit is set */
   F_BIT          = 0x40      /* disable IRQ when I bit is set */
   
/*
 * Register Base Address
 */
          
    .section .startup,"ax"
    .code 32
    
    .extern handleSWI
    
    .global _startup	/* the linker wants this symbol	*/
	
_startup:
        
/****************************************************************************/
/*               Vector table and reset entry                               */
/****************************************************************************/
_vectors:
   ldr pc, ResetAddr    /* Reset                 */
   ldr pc, UndefAddr    /* Undefined instruction */
   ldr pc, SWIAddr      /* Software interrupt    */
   ldr pc, PAbortAddr   /* Prefetch abort        */
   ldr pc, DAbortAddr   /* Data abort            */
   ldr pc, ReservedAddr /* Reserved              */
   ldr pc, [pc, #-0x120]       /* IRQ interrupt  */
   ldr pc, FIQAddr      /* FIQ interrupt         */


/* Jump Table for exceptions */
ResetAddr:     .word ResetHandler
UndefAddr:     .word UndefHandler
SWIAddr:       .word handleSWI
PAbortAddr:    .word PAbortHandler
DAbortAddr:    .word DAbortHandler
ReservedAddr:  .word 0
IRQAddr:       .word IRQHandler
FIQAddr:       .word FIQHandler

   .ltorg


   .section .init, "ax"
   .code 32
   
   .global ResetHandler
   .global ExitFunction
   .extern main
/****************************************************************************/
/*                           Reset handler                                  */
/****************************************************************************/
ResetHandler:
/*
    let the application stop processor advancement if they want
    to get gdb started at a known location.  after starting gdb
    the need to tell gdb jump past_gdb_hook
*/
   /*
    * Setup a stack for each mode
    */    
   msr   CPSR_c, #ARM_MODE_UNDEF | I_BIT | F_BIT   /* Undefined Instruction Mode */     
   ldr   sp, =__stack_und_end
   
   msr   CPSR_c, #ARM_MODE_ABORT | I_BIT | F_BIT   /* Abort Mode */
   ldr   sp, =__stack_abt_end
   
   msr   CPSR_c, #ARM_MODE_FIQ | I_BIT | F_BIT     /* FIQ Mode */   
   ldr   sp, =__stack_fiq_end
   
   msr   CPSR_c, #ARM_MODE_IRQ | I_BIT | F_BIT     /* IRQ Mode */   
   ldr   sp, =__stack_irq_end
   
   msr   CPSR_c, #ARM_MODE_SVC | I_BIT | F_BIT     /* Supervisor Mode */
   ldr   sp, =__stack_svc_end

   msr   CPSR_c, #ARM_MODE_SYS | I_BIT | F_BIT     /* System Mode */
   ldr   sp, =__stack_sys_end

/* start in SVC mode */
msr   CPSR_c, #ARM_MODE_SVC | I_BIT | F_BIT     /* Supervisor Mode */

/* 
 * Copy .data section (copy from ROM to RAM)
 */
     LDR     r1, =_etext
     LDR     r2, =__data_start
     LDR     r3, =__data_end
copyloop:   
     CMP     r2, r3
     LDRLO   r0, [r1], #4
     STRLO   r0, [r2], #4
     BLO     copyloop

/*
 * zero .bss section
 */
   ldr   r1, =__bss_start
   ldr   r2, =__bss_end
   ldr   r3, =0
bss_clear_loop:
   cmp   r1, r2
   strne r3, [r1], #+4
   bne   bss_clear_loop
   
   
   /*
    * Jump to main
    */
   mov   r0, #0 /* No arguments */
   mov   r1, #0 /* No arguments */
   ldr   r2, =main
   mov   lr, pc
   bx    r2     /* And jump... */
                       
ExitFunction:
   nop
   nop
   nop
   b ExitFunction   
   

/****************************************************************************/
/*                         Default interrupt handler                        */
/****************************************************************************/

UndefHandler:
   b UndefHandler
   
   /*
SWIHandler:
   b SWIHandler
   */

PAbortHandler:
   b PAbortHandler

DAbortHandler:
   b DAbortHandler
   
IRQHandler:
   b IRQHandler
   
FIQHandler:
   b FIQHandler
   
   .weak ExitFunction
   .weak UndefHandler, PAbortHandler, DAbortHandler
   .weak IRQHandler, FIQHandler

   .ltorg
/*** EOF ***/   
   

