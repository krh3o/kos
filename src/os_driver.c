/** 
 * 
 * \file os_driver.c
 *
 * Author: Karl L. Waters
 * Copyright (C) 2008 by Karl L. Waters
 * 
 * History:
 * 08 JUN 2008 : Created KLW
 * 
 */



//------------------------------------------------------------------------
// Includes
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "lpc-2378-stk.h"
#include "error_codes.h"
#include "os_swi.h"
#include "os_driver.h"


//------------------------------------------------------------------------
// Defines

//------------------------------------------------------------------------
// Typedefs
typedef enum DriverCallType_t {
	DRV_OPEN = 1,
	DRV_CLOSE,
	DRV_READ,
	DRV_WRITE,
	DRV_IOCTL
}DriverCallType_t;

typedef struct DriverCallData_t {
	DriverCallType_t type;
	void *pFunc;
	uint32_t arg1;
	uint32_t arg2;
	uint32_t arg3;
	uint32_t arg4;
	uint32_t rsvd;
}DriverCallData_t;

//------------------------------------------------------------------------
// Globals
#define MAX_DRIVER_CNT 32
DriverInfo_t *gpDriverList[MAX_DRIVER_CNT] = {0};
uint32_t gcDriverList = 0;



//------------------------------------------------------------------------
// Functions

// Driver Handle SWI call
uint32_t processSWI(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4)
{
	DriverCallData_t *pCallData = (DriverCallData_t*)arg1;
	switch (pCallData->type)
	{
	case DRV_OPEN:
		pCallData->arg1 = ((pDriverOpen_t)pCallData->pFunc)( (void*)pCallData->arg1, (uint32_t)pCallData->arg2 );
		break;
	case DRV_CLOSE:
		pCallData->arg1 = ((pDriverClose_t)pCallData->pFunc)( (void*)(pCallData->arg1) );
		break;
	case DRV_READ:
		pCallData->arg1 = ((pDriverRead_t)pCallData->pFunc)( (void*)pCallData->arg1, (void*)pCallData->arg2, (uint32_t*)pCallData->arg3 );
		break;
	case DRV_WRITE:
		pCallData->arg1 = ((pDriverWrite_t)pCallData->pFunc)( (void*)pCallData->arg1, (void*)pCallData->arg2, (uint32_t*)pCallData->arg3 );
		break;
	case DRV_IOCTL:
		pCallData->arg1 = ((pDriverIoctl_t)pCallData->pFunc)( (void*)pCallData->arg1, (uint32_t)pCallData->arg2, (void*)pCallData->arg3, (uint32_t*)pCallData->arg4 );
		break;
	}
	return 0;	
}

// Driver Register Function
uint32_t kos_RegisterDriver(DriverInfo_t *pDriverInfo)
{
	if (0==pDriverInfo)
	{
		return ERR_ARG;
	}
	if (gcDriverList>=MAX_DRIVER_CNT)
	{
		return ERR_DRIVER_FULL;
	}
	
	gpDriverList[gcDriverList] = pDriverInfo;
	gcDriverList++;
	
	return 0;
}
		
		
// Driver Open function 
uint32_t kos_DriverOpen(DriverHandle_t *Handle, void *pContext, uint32_t Flags)
{
	DriverCallData_t callData = {0};
	uint32_t cnt = gcDriverList;
	
	if (0==Handle)
	{
		return ERR_ARG;
	}
	while (cnt--)
	{
		if (0 == strcmp(gpDriverList[gcDriverList]->name, (char*)pContext))
		{
			*Handle = 0;
			
			callData.type = DRV_OPEN;
			callData.pFunc = gpDriverList[gcDriverList]->pOpen;
			callData.arg1 = (uint32_t)pContext;
			callData.arg2 = Flags;
			
			callSWI((uint32_t)&callData, 0, 0, 0);
			
			if (!CHECK_ERROR(callData.arg1))
			{
				*Handle = (DriverHandle_t)gcDriverList;
			}
			return callData.arg1;
		}
	}
		
	return ERR_DRIVER_NOT_FOUND;
}

// Driver Close function
uint32_t kos_DriverClose(DriverHandle_t Handle, void *pContext)
{
	DriverCallData_t callData = {0};
	
	if (Handle>gcDriverList)
	{
		return ERR_ARG;
	}
	
	callData.type = DRV_CLOSE;
	callData.pFunc = gpDriverList[gcDriverList]->pClose;
	callData.arg1 = (uint32_t)pContext;
	
	callSWI((uint32_t)&callData, 0, 0, 0);
	
	return callData.arg1;

}

// Driver Read function 
uint32_t kos_DriverRead(DriverHandle_t Handle, void *pContext, void *pBuffer, uint32_t *pByteCount)
{
	DriverCallData_t callData = {0};
	
	if (Handle>gcDriverList)
	{
		return ERR_ARG;
	}
	
	callData.type = DRV_READ;
	callData.pFunc = gpDriverList[gcDriverList]->pRead;
	callData.arg1 = (uint32_t)pContext;
	callData.arg2 = (uint32_t)pBuffer;
	callData.arg3 = (uint32_t)pByteCount;
	
	callSWI((uint32_t)&callData, 0, 0, 0);
	
	return callData.arg1;
}

// Driver Write function 
uint32_t kos_DriverWrite(DriverHandle_t Handle, void *pContext, void *pBuffer, uint32_t *pByteCount)
{
	DriverCallData_t callData = {0};
	
	if (Handle>gcDriverList)
	{
		return ERR_ARG;
	}
	
	callData.type = DRV_WRITE;
	callData.pFunc = gpDriverList[gcDriverList]->pWrite;
	callData.arg1 = (uint32_t)pContext;
	callData.arg2 = (uint32_t)pBuffer;
	callData.arg3 = (uint32_t)pByteCount;
	
	callSWI((uint32_t)&callData, 0, 0, 0);
	
	return callData.arg1;
}

// Driver Ioctl function 
uint32_t kos_DriverIoctl(DriverHandle_t Handle, void *pContext, uint32_t Control, void *pBuffer, uint32_t *pByteCount)
{
	DriverCallData_t callData = {0};
	
	if (Handle>gcDriverList)
	{
		return ERR_ARG;
	}
	
	callData.type = DRV_READ;
	callData.pFunc = gpDriverList[gcDriverList]->pRead;
	callData.arg1 = (uint32_t)pContext;
	callData.arg2 = Control;
	callData.arg3 = (uint32_t)pBuffer;
	callData.arg4 = (uint32_t)pByteCount;
	
	callSWI((uint32_t)&callData, 0, 0, 0);
	
	return callData.arg1;
}




/*
// Driver Read function blocking
uint32_t kos_DriverReadBlocking(DriverHandle_t Handle, void *pContext, void *pBuffer, uint32_t *pByteCount)
{
	return 0;
}

// Driver Write function blocking
uint32_t kos_DriverWriteBlocking(DriverHandle_t Handle, void *pContext, void *pBuffer, uint32_t *pByteCount)
{
	return 0;
}

// Driver Ioctl function blocking
uint32_t kos_DriverIoctlBlocking(DriverHandle_t Handle, void *pContext, uint32_t Control, void *pBuffer, uint32_t *pByteCount)
{
	return 0;
}
*/







