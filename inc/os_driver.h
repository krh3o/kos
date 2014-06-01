/**
 * 
 * \file os_driver.h
 *
 * Author: Karl L. Waters
 * Copyright (C) 2008 by Karl L. Waters
 * 
 * History:
 * 08 JUN 2008 : Created KLW
 * 
 */


#ifndef OS_DRIVER_H_
#define OS_DRIVER_H_


typedef uint32_t DriverHandle_t;

	// Driver Open function prototype
typedef uint32_t(*pDriverOpen_t)( void *pContext, uint32_t Flags);

	// Driver Close function prototype
typedef uint32_t(*pDriverClose_t)( void *pContext);

	// Driver Read function prototype
typedef uint32_t(*pDriverRead_t)( void *pContext, void *pBuffer, uint32_t *pByteCount);

	// Driver Write function prototype
typedef uint32_t(*pDriverWrite_t)( void *pContext, void *pBuffer, uint32_t *pByteCount);

	// Driver Ioctl function prototype
typedef uint32_t(*pDriverIoctl_t)( void *pContext, uint32_t Control, void *pBuffer, uint32_t *pByteCount);


typedef struct DriverInfo_t
{
	char name[12];
	pDriverOpen_t pOpen;
	pDriverClose_t pClose;
	pDriverRead_t pRead;
	pDriverWrite_t pWrite;
	pDriverIoctl_t pIoctl;
	
}DriverInfo_t;


	// Driver register new driver
uint32_t kos_RegisterDriver(DriverInfo_t *pDriverInfo);

	// Driver Open function 
uint32_t kos_DriverOpen(DriverHandle_t *Handle, void *pContext, uint32_t Flags);

	// Driver Close function
uint32_t kos_DriverClose(DriverHandle_t Handle, void *pContext);

	// Driver Read function 
uint32_t kos_DriverRead(DriverHandle_t Handle, void *pContext, void *pBuffer, uint32_t *pByteCount);

	// Driver Write function 
uint32_t kos_DriverWrite(DriverHandle_t Handle, void *pContext, void *pBuffer, uint32_t *pByteCount);

	// Driver Ioctl function 
uint32_t kos_DriverIoctl(DriverHandle_t Handle, void *pContext, uint32_t Control, void *pBuffer, uint32_t *pByteCount);

/*
	// Driver Read function blocking
uint32_t kos_DriverReadBlocking(DriverHandle_t Handle, void *pContext, void *pBuffer, uint32_t *pByteCount);

	// Driver Write function blocking
uint32_t kos_DriverWriteBlocking(DriverHandle_t Handle, void *pContext, void *pBuffer, uint32_t *pByteCount);

	// Driver Ioctl function blocking
uint32_t kos_DriverIoctlBlocking(DriverHandle_t Handle, void *pContext, uint32_t Control, void *pBuffer, uint32_t *pByteCount);
*/


#endif /*OS_DRIVER_H_*/
