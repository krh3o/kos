/*
 * DESCRIPTION:
 *
 *  Olimex LPC-2378-STK specific definitions
 *
 */

#ifndef _LPC_2378_STK
#define _LPC_2378_STK

#include "lpc2378.h"


/* TODO: change to use structs
 * Board-specific stuff goes here */
//#define   BUT1        IO0PIN_bit.P0_15 /* BUT1 P0.29 */
//#define   BUT1_dir    IO0DIR_bit.P0_15
//#define   BUT2        IO0PIN_bit.P0_16 /* BUT2 P0.18*/
//#define   BUT2_dir    IO0DIR_bit.P0_16
//#define   LED1_on     IO0CLR_bit.P0_10 /* LED1 */
//#define   LED1_off    IO0SET_bit.P0_10
//#define   LED1_dir    IO0DIR_bit.P0_10
//#define   LED2_on     IO0CLR_bit.P0_11 /* LED2 */
//#define   LED2_off    IO0SET_bit.P0_11
//#define   LED2_dir    IO0DIR_bit.P0_11
//#define   nCP         IO1PIN_bit.P0_24 /* MMC CP */
//#define   nCP_dir     IO1DIR_bit.P0_24
//#define   WP          IO1PIN_bit.P0_25 /* MMC WP */
//#define   WP_dir      IO1DIR_bit.P0_25


//#define   CCS_high    IO0SET_bit.P0_2
//#define   CCS_low     IO0CLR_bit.P0_2
//#define   CCS_dir     IO0DIR_bit.P0_2
#define   CCS_BIT     BIT(28)           /* UEXT-5 LPC2378 P0.28 */
//#define   DCS_high    IO0SET_bit.P0_7
//#define   DCS_low     IO0CLR_bit.P0_7
//#define   DCS_dir     IO0DIR_bit.P0_7
#define   DCS_BIT     BIT(6)            /* UEXT-10 LPC2378 P0.6 */
//#define   EEBOOT_high IO0SET_bit.P0_3
//#define   EEBOOT_low  IO0CLR_bit.P0_3
//#define   EEBOOT_dir  IO0DIR_bit.P0_3
#define   EEBOOT_BIT  BIT(27)           /* UEXT-6 LPC2378 P0.27 */
//#define   MP3RES_high IO0SET_bit.P0_9
//#define   MP3RES_low  IO0CLR_bit.P0_9
//#define   MP3RES_dir  IO0DIR_bit.P0_9
#define   MP3RES_BIT  BIT(11)           /* UEXT-4 LPC2378 P0.11 */

//#define   DREQ        IO0PIN_bit.P0_8
//#define   DREQ_dir    IO0DIR_bit.P0_8
#define   DREQ_BIT    BIT(10)           /* UEXT-3 LPC2378 P0.10 */


#define SD_LED_SET          FIO0SET
#define SD_LED_DIR          FIO0DIR
#define SD_LED_CLR          FIO0CLR
#define SD_LED_PIN          FIO0PIN
#define SD_LED_BIT          BIT(21)

#define USBC_LED_BIT        BIT(14)
#define USB_LINK_LED_BIT    BIT(13)

#define LCD_BL_BIT          BIT(26)


#define JOYSTICK_LEFT   (1 << 27)
#define JOYSTICK_CENTER (1 << 25)
#define JOYSTICK_DOWN   (1 << 19)
#define JOYSTICK_UP     (1 << 18)
#define JOYSTICK_RIGHT  (1 << 22)
#define JOYSTICK_PINS   (JOYSTICK_LEFT | JOYSTICK_CENTER | JOYSTICK_DOWN | JOYSTICK_UP | JOYSTICK_RIGHT)


#endif /* __LPC_2378_STK__ */
