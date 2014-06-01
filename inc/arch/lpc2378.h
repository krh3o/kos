/*
 * MODULE:
 *
 *  lpc2378.h
 *
 * DESCRIPTION:
 *
 *  LPC2378 on-chip specific standard definitions file
 *
 */

#ifndef __LPC2378_H
#define __LPC2378_H


/* TODO move these defines and types up to an architecture include file */
/* Attributes for hardware register struct alignment and packing
 * http://gcc.gnu.org/onlinedocs/gcc/Type-Attributes.html*/
#define ATTR_PACKED_ALIGNED4  __attribute__ ((__packed__,__aligned__(4)))
#define ATTR_PACKED           __attribute__ ((__packed__))

/* Function pointer type */
typedef void (*pfunction_t)(void);



#include "common.h"

#include "dac.h"
#include "gpio.h"
#include "pinconnect.h"
#include "scb.h"
#include "timers.h"
#include "uarts.h"
#include "vic.h"



/* Memory Locations */
#define FLASH_BASE     0x00000000
#define FLASH_SIZE     0x0007E000   /* 516,096 bytes */
                                    /*   8,192 bytes boot block at end of flash */

#define SRAM_BASE      0x40000000
#define SRAM_SIZE      0x00008000   /* 32,768 bytes */


#endif /* __LPC2378_H */
