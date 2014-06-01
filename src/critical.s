.global InterruptsDisable
.global InterruptsEnable
.global InterruptsRestore

.set ARM_SR_DISABLE_FIQ_AND_IRQ,         0xC0   /* Disable both FIQ & IRQ */
.set ARM_SR_BIT_IRQ,                     0x80   /* IRQ bit */

.text
.arm

/* uint32_t InterruptsDisable(void); */
InterruptsDisable:
	MRS r0, CPSR                    			/* get CPSR */
    ORR r1, r0, #ARM_SR_DISABLE_FIQ_AND_IRQ     /* set IRQ bit - disable IRQ */
    MSR CPSR_c, r1                  			/* load CPSR */
    bx lr


/* void InterruptsEnable(uint32_t cpu_sr); */
InterruptsEnable:
	BIC r0, r0, #ARM_SR_DISABLE_FIQ_AND_IRQ    /* clear IRQ bit */
	MSR CPSR_c, r0                 				/* load CPSR */
	bx lr

/* void InterruptsRestore(uint32_t cpu_sr); */
InterruptsRestore:
	MSR CPSR_c, r0                 				/* load CPSR */
	bx lr
	
.end
