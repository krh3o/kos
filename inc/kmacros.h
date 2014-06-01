/*
 * SIMPLE LITTLE OPERATING SYSTEM - SLOS
 */

/*
 * FILE
 *
 *  kmacro.s
 *
 * DESCRIPTION
 *
 *  Provides the standard marcros and makes world consistent between all
 *  low level handlers and UES
 *
 * OS
 *
 *  SLOS REV.00000016
 *
 * PLATFORM
 *
 *  generic
 *
 * ORIGINATOR
 *
 *  Andrew N. Sloss
 *
 * HISTORY
 * 
 * - added thread ID to the frame
 * - move into a separate file 
 * - August 30 2003
 * 
 * NOTES
 * 
 */

/*
 * MACROS
 */

/*
 *  register aliases
 */

     r13_irq .req r13
     r12_fiq .req r12
     r13_fiq .req r13
     r13_svc .req r13
     r14_svc .req r14
     r13_usr .req r13
     r14_usr .req r14
     r13_abt .req r13
     r13_und .req r13
     r13_sys .req r13
     r14_sys .req r14

/*
 * mode macros
 */
        
     .equ modeMask, 0x1f                       @ processor mode mask
     .equ modeSVC, 0x13                        @ SVC mode
     .equ modeIRQ, 0x12                        @ IRQ mode
     .equ modeFIQ, 0x11                        @ FIQ mode
     .equ modeSYS, 0x1f                        @ SYSTEM mode
     .equ modeUSR, 0x10                        @ User mode
     .equ modeABT, 0x17                        @ Abort mode
     .equ modeUND, 0x1b                        @ undefined mode

/*
 * interrupt bits
 */
        
     .equ iFt, 0x40                            @ IRQ:en FIQ:dis:ARM state
     .equ IFt, 0xc0                            @ IRQ:dis FIQ:dis:ARM state

/*
 * frame offsets
 */
        
     .equ frameR0, 0x00	                          @ [00] +0x00		
     .equ frameR1, frameR0+4                      @ [01] +0x04
     .equ frameR2, frameR1+4                      @ [02] +0x08
     .equ frameR3, frameR2+4                      @ [03] +0x0c
     .equ frameR4, frameR3+4                      @ [04] +0x10 
     .equ frameR5, frameR4+4                      @ [05] +0x14
     .equ frameR6, frameR5+4                      @ [06] +0x18
     .equ frameR7, frameR6+4                      @ [07] +0x1c
     .equ frameR8, frameR7+4                      @ [08] +0x20
     .equ frameR9, frameR8+4                      @ [09] +0x24
     .equ frameR10, frameR9+4                     @ [10] +0x28
     .equ frameR11, frameR10+4                    @ [11] +0x2c
     .equ frameR12, frameR11+4                    @ [12] +0x30
     .equ frameR13, frameR12+4                    @ [13] +0x34
     .equ frameLR, frameR13+4                     @ [14] +0x38
     .equ framePC, frameLR+4                      @ [15] +0x3c
     .equ framePSR, framePC+4                     @ [16] +0x40
     .equ frameSVCR13, framePSR+4                 @ [17] +0x44
     .equ frameID, frameSVCR13+4                  @ [18] +0x48
     .equ frameSIZE, frameID+4                    @ [19] +0x4c

/*
 * return codes
 */

   .equ userIRQ, 0
   .equ userFIQ, 1
   .equ userData, 2
   .equ userPrefetch, 3
   .equ userUndefined, 4
   .equ userSWI, 5

  