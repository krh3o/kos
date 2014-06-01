/** 
 * 
 * \file error_codes.c
 *
 * Author: Karl L. Waters
 * Copyright (C) 2008 by Karl L. Waters
 * 
 * History:
 * 01 APR 2008 : Created KLW
 * 08 JUN 2008 : Updated KLW
 * 
 */




#ifndef ERROR_CODES_H_
#define ERROR_CODES_H_



#define OS_NO_ERR	0x0

#define NO_ERROR	0x0

#define ERROR_BASE		0x80000000

//------------------------------------------------------
// OS Error Codes
#define OS_ERROR_BASE		0x0
#define OS_ERR				(ERROR_BASE|(OS_ERROR_BASE+1))

//------------------------------------------------------
// General Error Codes
#define GENERAL_ERROR_BASE		0x100
#define ERR_ARG					(ERROR_BASE|(GENERAL_ERROR_BASE+1))

//------------------------------------------------------
// Driver Error Codes
#define DRIVER_ERROR_BASE		0x200
#define ERR_DRIVER_FULL			(ERROR_BASE|(DRIVER_ERROR_BASE+1))
#define ERR_DRIVER_NOT_FOUND	(ERROR_BASE|(DRIVER_ERROR_BASE+2))


//------------------------------------------------------
// Error Handling Macros
#define CHECK_ERROR(x)	(((x)&ERROR_BASE) == ERROR_BASE)


#endif /*ERROR_CODES_H_*/
