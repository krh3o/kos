/** 
 * 
 * \file drv_test.c
 * Dummy driver for test purposes
 *
 * Author: Karl L. Waters
 * Copyright (C) 2008 by Karl L. Waters
 * 
 * History:
 * 08 JUN 2008 : Created KLW
 * 
 */


#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "lpc-2378-stk.h"
#include "error_codes.h"
#include "os_driver.h"

#include "printf.h"

#include "drv_test.h"

//------------------------------------------------------------------------
// Local Function Prototypes
static uint32_t drvtest_open( void *pContext, uint32_t Flags);
static uint32_t drvtest_close( void *pContext);
static uint32_t drvtest_read( void *pContext, void *pBuffer, uint32_t *pByteCount);
static uint32_t drvtest_write( void *pContext, void *pBuffer, uint32_t *pByteCount);
static uint32_t drvtest_ioctl( void *pContext, uint32_t Control, void *pBuffer, uint32_t *pByteCount);


//------------------------------------------------------------------------
// Globals
DriverInfo_t gDrvTestInfo = {TESTDRIVER, drvtest_open, drvtest_close, drvtest_read, drvtest_write, drvtest_ioctl};



//------------------------------------------------------------------------
// Functions
uint32_t drvtest_install( void )
{
	
	return kos_RegisterDriver(&gDrvTestInfo);
}

	// Driver Open function prototype
static uint32_t drvtest_open( void *pContext, uint32_t Flags)
{
	printf("\n\r%s: called\n\r", __FUNCTION__);
	return NO_ERROR;
}

	// Driver Close function prototype
static uint32_t drvtest_close( void *pContext)
{
	printf("\n\r%s: called\n\r", __FUNCTION__);
	return NO_ERROR;
}

	// Driver Read function prototype
static uint32_t drvtest_read( void *pContext, void *pBuffer, uint32_t *pByteCount)
{
	printf("\n\r%s: called\n\r", __FUNCTION__);
	return NO_ERROR;
}

	// Driver Write function prototype
static uint32_t drvtest_write( void *pContext, void *pBuffer, uint32_t *pByteCount)
{
	printf("\n\r%s: called\n\r", __FUNCTION__);
	return NO_ERROR;
}

	// Driver Ioctl function prototype
static uint32_t drvtest_ioctl( void *pContext, uint32_t Control, void *pBuffer, uint32_t *pByteCount)
{
	printf("\n\r%s: called\n\r", __FUNCTION__);
	return NO_ERROR;
}


