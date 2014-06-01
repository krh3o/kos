/** 
 * 
 * \file os_core.c
 * Karl's Operating System (kos)
 *
 * Author: Karl L. Waters
 * Copyright (C) 2008 by Karl L. Waters
 * 
 * History:
 * 01 APR 2008 : Created KLW
 * 
 */

#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "lpc-2378-stk.h"
#include "error_codes.h"
#include "safe_strings.h"
#include "init.h"
#include "os_core.h"

#include "printf.h"

//--------------------------------------------------------------
// external functions

extern void TimerTickISR(void);

extern void Restore_Context(void);

extern uint32_t InterruptsDisable(void);
extern void InterruptsRestore(uint32_t cpsr);
extern void InterruptsEnable(uint32_t cpsr);

extern void TestISR(void);

//--------------------------------------------------------------
// defines

#define KOS_MAX_THREAD_NAME_LEN 12
#define KOS_MAX_THREADS 12

#define KOS_TICKS_PER_SEC 100

//--------------------------------------------------------------
// typedefs

typedef enum threadState_t
{
	thread_active = 0,
	thread_ready,
	thread_waiting
}threadState_t;


typedef struct threadTCB_t {
	volatile KOS_STK *stack; // must be first in TCB
	uint32_t pri;
	uint32_t id;
	threadState_t state;
	char	name[KOS_MAX_THREAD_NAME_LEN];
	uint32_t stackSize;
	struct threadTCB_t *pNext;
}threadTCB_t, *pthreadTCB_t;



//--------------------------------------------------------------
// file local variables

threadTCB_t *kos_threadCurr = 0;

threadTCB_t *kos_threadList[KOS_MAX_PRIORITIES] = {0};

//--------------------------------------------------------------
// file local variables

static uint8_t kos_threadIdInc = 0;

static BOOL kos_initialized = FALSE;

static uint32_t globalTime = 0;

//--------------------------------------------------------------
// local function prototypes
//static void OutPutThreadStates(void);
static void kos_IdleThread(void *pData);
static void Tmr_TickInit (void);
static uint32_t kos_InitThreadStack( KOS_STK **ppStk, uint32_t size, threadfunc_t *pFunc, void *pVoid);

//--------------------------------------------------------------

void kos_TimerTick(void);
void kos_ScheduleNext(void);


//--------------------------------------------------------------
// functions


/**
 * Initializes the system clock.
 * 
 * Local to this file, called only by the OS.
 */
static void Tmr_TickInit (void)
{
    uint32_t pclk_freq;
    uint32_t rld_cnts;
    uint32_t cclk_freq;
    uint32_t pclk_sel;
    uint32_t pclk_div;

    // VIC timer #0 Initialization 
    P_VIC_REGS->IntSelect = P_VIC_REGS->IntSelect & ~(1<<VIC_CH4_TIMER0);	// Configure the timer interrupt as an IRQ source
    
    P_VIC_REGS->VectAddr4 = TimerTickISR;							// Set the vector address
    // P_VIC_REGS->VectAddr4 = TestISR;
    
    P_VIC_REGS->VectPriority4 = 2;

    // Get the peripheral clock frequency
    // find out pclk divider
    pclk_sel = GET_PCLK_SEL( P_SCB_REGS->PCLKSEL0, PCLK_TIMER0 );
    pclk_div = ( pclk_sel == 0 ? 4 : \
                 pclk_sel == 1 ? 1 : \
                 pclk_sel == 2 ? 2 : \
                 pclk_sel == 3 ? 8 : \
                 0 ); // error
    if( pclk_div == 0 )
    {
        return;
    }
    cclk_freq = getFcclk();
    pclk_freq = cclk_freq/pclk_div;

    rld_cnts      =   pclk_freq / KOS_TICKS_PER_SEC;	// Calculate the # of counts necessary for the OS ticker

    P_TIMER0_REGS->TCR = (1 << 1);			// Disable and reset counter 0 and the prescale counter 0
    P_TIMER0_REGS->TCR = 0;					// Clear the reset bit
    P_TIMER0_REGS->PR = 1000;					// Prescaler is set to no division

    P_TIMER0_REGS->MR0 = rld_cnts;
    P_TIMER0_REGS->MCR = 3;					// Interrupt on MR0 (reset TC), stop TC

    P_TIMER0_REGS->CCR = 0;					// Capture is disabled.
    P_TIMER0_REGS->EMR = 0;					// No external match output.
    P_TIMER0_REGS->TCR = 1;					// Enable timer 0
    
    P_VIC_REGS->IntEnable = (1<<VIC_CH4_TIMER0);				// Enable the timer interrupt source
}


/**
 * kos_TimerTick increments OS clock and resets the timer interrupts.
 */
void kos_TimerTick(void)
{
	globalTime++;
	P_TIMER0_REGS->IR = 1;	// reset timer interrupt
	P_VIC_REGS->Address = (pfunction_t)0xFF; // reset vic
	//do something else?
}

/**
 * Schedules the next TCB.
 */
void kos_ScheduleNext(void)
{
    uint32_t pri = 0;
    
    // last array element must be idle thread so it will never go out of bounds
    // bigger problems if there is no idle thread
    while (!(kos_threadList[pri]))
    {
        pri++;
    }
    
    kos_threadList[pri] = kos_threadList[pri]->pNext;
    
	kos_threadCurr =  kos_threadList[pri];
}

#define STACK_SIZE_IDLE 	(200+sizeof(threadTCB_t))
KOS_STK __attribute__ ((__aligned__(4))) threadStackIdle[STACK_SIZE_IDLE] = {0};
/** 
 * Idle thread.
 * 
 * Busy waits with a while loop.
 * 
 * @param pData not used
 */
static void kos_IdleThread(void *pData)
{
	pData = pData;
	volatile uint32_t cnt;
	
	while (1) {
		
		printf("Idle looped. Time:%d\n\r", globalTime);
		cnt = 0xFFF;
		while ( cnt-- ) {}
	}
	
}


/**** Public Functions ****/

/*
 * Initialize the OS. Documented in os_core.h
 */
uint32_t kos_InitOS(void)
{
	uint32_t err = OS_NO_ERR;
	
	kos_initialized = TRUE;
	
	err = kos_CreateThread( KOS_LOWEST_PRIORITY, "Idle Thread", threadStackIdle, STACK_SIZE_IDLE, kos_IdleThread, 0);
	
	if (OS_NO_ERR!=err)
	{
		kos_initialized = FALSE;
		return err;
	}
	
	return err;
}

/*
 * Adds a thread function to the schedular. Documented in os_core.h
 */
uint32_t kos_CreateThread( uint8_t pri, const char* pszName, KOS_STK *stack, uint32_t stk_size, threadfunc_t *pThreadFunc, void *pVoid)
{

	uint32_t err = OS_NO_ERR;
	//uint8_t i = 0;
	uint32_t cpsr = 0;
	
	threadTCB_t *newTask = (threadTCB_t*)(stack);
	
	if ((0 == pThreadFunc) || (0 == stack)) {
		return OS_ERR;
	}
	
	if (!kos_initialized) {
		return OS_ERR;
	}
	
	if (stk_size < sizeof(threadTCB_t))
	{
		return OS_ERR;
	}
	
	if (pri>KOS_LOWEST_PRIORITY)
	{
	    return OS_ERR;
	}
	
	newTask->id = kos_threadIdInc++;
	
	newTask->pri = pri;
	
	stack = stack+stk_size-1;
	err = kos_InitThreadStack( &(stack), stk_size-sizeof(threadTCB_t), pThreadFunc, pVoid);
	if (err != OS_NO_ERR)
		return err;
			
	newTask->stack = stack;
	newTask->stackSize = stk_size-sizeof(threadTCB_t);
		
	newTask->state = thread_ready;
	
	if (0 != pszName)
	{
		strlcpy(newTask->name, pszName, KOS_MAX_THREAD_NAME_LEN);
	}
	
	// add new task to tasks list
	
	cpsr = InterruptsDisable(); // must lock out schedular while changing task lists
	
	if (0 == kos_threadList[pri])
	{
		kos_threadList[pri] = newTask;
		newTask->pNext = newTask;
	}
	else
	{
		newTask->pNext = kos_threadList[pri]->pNext;
		kos_threadList[pri]->pNext = newTask;
		kos_threadList[pri] = newTask;  // list head points to most recently added task
	}
	
	InterruptsRestore(cpsr);
	
	return OS_NO_ERR;
}

// some sort of sleep/pause function
// not busy wait, should it context switch before the tick? no for now.
uint32_t kos_sleep(uint32_t ticks)
{
    // put the thread to sleep for a set number of ticks
    // will have to call timer tick if I want to immediately advance to the next thread
    // need to create some sort of 'events' list to monitor
    return 0;
}

// semaphore/mutex create

// semaphore/mutex delete

// semaphore/mutex wait

// semaphore/mutex trywait

// semaphore/mutex signal

// semaphore/mutex peek


/**** End Public Functions ****/



#define  ARM_MODE_ARM           0x00000000
#define  ARM_MODE_THUMB         0x00000020


#define ARM_MODE_USER   0x10      // Normal User Mode                              
#define ARM_MODE_FIQ    0x11      // FIQ Fast Interrupts Mode                     
#define ARM_MODE_IRQ    0x12      // IRQ Standard Interrupts Mode                 
#define ARM_MODE_SVC    0x13      // Supervisor Interrupts Mode                   
#define ARM_MODE_ABORT  0x17      // Abort Processing memory Faults Mode          
#define ARM_MODE_UNDEF  0x1B      // Undefined Instructions Mode                  
#define ARM_MODE_SYS    0x1F      // System Running in Priviledged Operating Mode 
#define ARM_MODE_MASK   0x1F

/**
 * Initialize a task's stack for a context switch.
 * 
 * This is called from kos_CreateThread to initialize the TCB.
 * It places everything that is needed on the stack to create the initial task context.
 */
static uint32_t kos_InitThreadStack( KOS_STK **ppStk, uint32_t size, threadfunc_t *pFunc, void *pVoid)
{
	
	if ((0==ppStk) || (0==*ppStk) || (0==pFunc))
	{
		return OS_ERR;
	}
	
	KOS_STK *pStk = *ppStk;
	
	if (size < 20)
	{
		return OS_ERR;
	}
	
	*pStk		= (uint32_t)pFunc;			// R15 = PC - Task Entry Point
	*(--pStk)	= (uint32_t)0x0;			// R14 = lr - should never return from thread
	*(--pStk)	= (uint32_t)*ppStk;			// R13 = sp - point to original base of stack - all this will be popped in context restore
	*(--pStk)	= (uint32_t)0x12121212;		// R12
	*(--pStk)	= (uint32_t)0x11111111;		// R11
	*(--pStk)	= (uint32_t)0x10101010;		// R10
	*(--pStk)	= (uint32_t)0x09090909;		// R09
	*(--pStk)	= (uint32_t)0x08080808;		// R08
	*(--pStk)	= (uint32_t)0x07070707;		// R07
	*(--pStk)	= (uint32_t)0x06060606;		// R06
	*(--pStk)	= (uint32_t)0x05050505;		// R05
	*(--pStk)	= (uint32_t)0x04040404;		// R04
	*(--pStk)	= (uint32_t)0x03030303;		// R03
	*(--pStk)	= (uint32_t)0x02020202;		// R02
	*(--pStk)	= (uint32_t)0x01010101;		// R01
	*(--pStk)	= (uint32_t)pVoid;			// R00 = argument passed in - in this case what ever void pointer was passed in using CreateThread
	
    if ((uint32_t)pFunc & 0x01) {								// check if task is ARM or THUMB mode
        *(--pStk) = (uint32_t)(ARM_MODE_USER|ARM_MODE_THUMB);	// CPSR  (Enable both IRQ and FIQ interrupts, THUMB-mode)
    } else {
        *(--pStk) = (uint32_t)ARM_MODE_USER;					// CPSR  (Enable both IRQ and FIQ interrupts, ARM-mode)
    }
    
    *ppStk = pStk;
    
    return OS_NO_ERR;
}



/*
 * Start the OS. Documented in os_core.h
 */
 uint32_t kos_StartOS(void)
 {
	 		 
	if (!kos_initialized) {
		return OS_ERR;
	}
	
	Tmr_TickInit();
	
	// Restore_Context must not be called in sys or usr mode
	Restore_Context();
	 
	// should never return
	 return OS_NO_ERR;
 }







