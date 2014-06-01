/**
 * 
	Copyright (c) 2008  Karl Waters
	
	Module Name:
	    main.c
	
	Module Description:
	    the main function, which exits when the program terminates
	
	History:	
	    Created April 2008 KW

**/

#define __MAIN_C__

#include <stdint.h>
#include "lpc-2378-stk.h"
#include "init.h"
#include "printf.h"
//#include "print.h"

#include "os_core.h"
#include "os_driver.h"

#include "drv_test.h"

#include "app.h"





extern uint32_t InterruptsDisable(void);
extern void InterruptsEnable(uint32_t cpsr);


extern void _startup(void); // for debugging startup code




/******************************************************************************
 * Utilities
 */

/** 
 * A while loop delay.
 * 
 * @param cnt while loop count down
 */
void delay(volatile uint32_t cnt)
{
    volatile uint32_t cnt2 = 100;
    while(cnt2)
    {
        while(cnt){cnt--;}
        cnt2--;
    }
}

/** 
 * Write a charater to the uart register
 * 
 * @param p does nothing
 * @param c is the value to write to the uart
 */
void putc_ex ( void* p, char c)
{
#if (SERIALPRINT_UARTNUM == 0)
	
	while(!(P_UART0_REGS->LSR & ULSR_THRE)) continue;
	
	P_UART0_REGS->THR = (uint8_t) c;
	
#elif (SERIALPRINT_UARTNUM == 2)
	
	while(!(P_UART2_REGS->LSR & ULSR_THRE)) continue;
	
	P_UART2_REGS->THR = (uint8_t) c;
	
#endif
}


/******************************************************************************
 * Main
 */
/*++

Routine Description:

    Standard program main entry point.

Arguments:

    None.

Return Value:

    Returns 0, which indicates nothing.

--*/
int main( void )
{
    /* Initial SCB Parameters */
    scb2300_t SCBParams = {
        .PLL_M_Mul = 12,           /* PLL Multiplier. Valid values 6 through 512*/
        .PLL_N_Div = 1,            /* PLL Divider. Valid values 1 through 32 */
        .PLL_Fcco = 288000000,     /* Frequency (Hz) of PLL output */
        .CCLK_Div = 6,             /* CPU Clock divider, cclk */
        .MAMMode = MAMCR_PARTIAL,  /* MAM mode Partial is the preferred setting for Rev -,A parts */
        .MAMTim = MAMTIM_AUTOCFG   /* Let initMAM calculate the optimal MAM timing */
    };

    //uint32_t cpu_sr;

    // Initialize System
    initHardware(&SCBParams);
    
    /* don't enable interrupts
     *   - those will be enabled by the task when it's cpsr is restored. 
     */
    /* enable interrupts 
    cpu_sr = InterruptsDisable();
    InterruptsEnable(cpu_sr);
    */

    
    init_printf(0,putc_ex);
    
    /***************************************************************************
     * Uncomment this to debug _startup() in crt.s 
     * This call must be after initHardware since MEMMAP needs to be configured 
     */
//    _startup();
    

    printf("\033[2J");
    //while (1)
    {
    printf("\n\r\n\rStarting...\n\r");
    }

    //--------------------------------------------------------------------------
    // install the dummy driver
    {
    	uint32_t retVal = drvtest_install();
    	printf("\n\rDummy Driver Installed : 0x%08X\n\r", retVal);
    }    
    
    /***************************************************************************
     *  Run Operating System
     */
    
    kos_InitOS();
    
    kos_CreateThread(  25, "Thread 1", thread1Stack, STACK_SIZE, thread1Entry, (void*)&shared);
    kos_CreateThread(  25, "Thread 2", thread2Stack, STACK_SIZE, thread2Entry, (void*)&shared);
    kos_CreateThread( 100, "Thread 3", thread3Stack, STACK_SIZE, thread3Entry, (void*)&shared);
    
    kos_StartOS();



    // Should never get here
    printf("Exiting...\n\r");
    return 0;
}

/*** EOF ***/
