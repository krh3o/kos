
/**
 *  app.c
 * 
 *  Applicatioin for kos
 * 
 *  Author
 * 		Karl Waters - April 2008
 */

#include <stdint.h>
#include "error_codes.h"
#include "os_core.h"

#include "os_driver.h"

#include "drv_test.h"

#include "app.h"
#include "printf.h" 


// thread stacks
KOS_STK __attribute__ ((__aligned__(4))) thread1Stack[STACK_SIZE] = {0};

KOS_STK __attribute__ ((__aligned__(4))) thread2Stack[STACK_SIZE] = {0};

KOS_STK __attribute__ ((__aligned__(4))) thread3Stack[STACK_SIZE] = {0};




GlobalDataStr_t shared = {0};

DriverHandle_t gDrvHandle = 0;

/** 
 * Thread 1.
 * 
 * Casts pData to GlobalDataStr_t and increments the variable inc
 * 
 * @param pData is a pointer to shared data
 * 
 */
void thread1Entry(void *pData)
{
	GlobalDataStr_t *s = (GlobalDataStr_t *)pData;
	volatile uint32_t cnt = 0;
	uint32_t retVal = 0;
	uint32_t byteCount = 16;
	uint8_t pBytes[16] = {5,6,7,8,9,12,0,0,0,250,0,0,0,0,0,0};
	
	if (!gDrvHandle)
	{
		retVal = kos_DriverOpen(&gDrvHandle, TESTDRIVER, 0);
	}
	
	while (1)
	{
		printf("1t\n\r");
		
		s->t1++;
		
		if (s->guard==0)
		{
			uint32_t tmp = byteCount;
			s->guard = 1;
			kos_DriverWrite(gDrvHandle, 0, pBytes, &tmp);
			s->inc++;
			s->guard = 0;
		}
		cnt = 0x8FFF;
		while ( cnt-- ) {}
	}
	
}


/** 
 * Thread 2.
 * 
 * Casts pData to GlobalDataStr_t and decrements the variable inc
 * 
 * @param pData is a pointer to shared data
 * 
 */
void thread2Entry(void *pData)
{
	GlobalDataStr_t *s = (GlobalDataStr_t *)pData;
	volatile uint32_t cnt = 0;
	
	while (1)
	{
		printf("2t\n\r");
		
		s->t2++;
		
		if (s->guard==0)
		{
			s->guard = 1;
			s->inc--;
			s->guard = 0;
		}
		
		cnt = 0xFFF;
		while ( cnt-- ) {}
	}
	
}

/** 
 * Thread 3.
 * 
 * Casts pData to GlobalDataStr_t and checks the inc variable.
 * Returns error if inc is larger then 3.
 * 
 * @param pData is a pointer to shared data
 * 
 */
void thread3Entry(void *pData)
{
	GlobalDataStr_t *s = (GlobalDataStr_t *)pData;
	volatile uint32_t cnt = 0;
	
	while (1)
	{
		printf("3t\n\r");
		
		if (s->guard==0)
		{
			s->guard = 1;
			if ( s->inc > 0x7F ) {
				s->inc = 0;
			}
			s->guard = 0;
		}
		
		cnt = 0xFFF;
		while ( cnt-- ) {}
	}
	
}


