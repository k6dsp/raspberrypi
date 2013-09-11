;;  Copyright ARM Ltd 2002-2010. All rights reserved.
;;
;;  This code initialises the Versatile board (eg REMAP) before calling
;;  TCM Initialization and MMU Initialization if they exist.
;;  this allows scatter loading to relocate code into the TCMs
;;
;;  This code must be run from a privileged mode

    PRESERVE8

; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_USR        EQU     0x10
Mode_FIQ        EQU     0x11
Mode_IRQ        EQU     0x12
Mode_SVC        EQU     0x13
Mode_ABT        EQU     0x17
Mode_UND        EQU     0x1B
Mode_SYS        EQU     0x1F

I_Bit           EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40 ; when F bit is set, FIQ is disabled

Len_IRQ_Stack   EQU      256 ; IRQ stack size

; --- System memory locations

Versatile_ctl_reg       EQU     0x101E0000          ; Address of Versatile Control Register
DEVCHIP_Remap_bit       EQU     0x100               ; Bit 8 is remap bit of Control Register

        AREA   INIT, CODE, READONLY   ; name this block of code

        ENTRY

        EXPORT  Reset_Handler
Reset_Handler   FUNCTION

; --- Perform ROM/RAM remapping, if required
    IF :DEF: ROM_RAM_REMAP

; On reset, an aliased copy of ROM is at 0x0.
; Continue execution from 'real' ROM rather than aliased copy
        LDR     pc, =Instruct_2

Instruct_2

; Remap by setting Remap bit of the control register
; Clear the DEVCHIP REMAP by writing 1 to bit 8 of the system control register
        LDR     r1, =Versatile_ctl_reg
        LDR     r0, [r1]
        ORR     r0, r0, #DEVCHIP_Remap_bit
        STR     r0, [r1]

; RAM is now at 0x0.
; The exception vectors (in vectors.s) must be copied from ROM to the RAM
; The copying is done later by the C library code inside __main

    ENDIF


; Setup stacks for exception handling modes
; exception_stack_base could be defined below, or located in a scatter file
        LDR     r0, exception_stack_base

; Enter each needed exception mode in turn and set up the stack pointer
        MSR     CPSR_c, #Mode_IRQ:OR:I_Bit:OR:F_Bit
        MOV     sp, r0

        SUB     r0, r0, #Len_IRQ_Stack

        MSR     CPSR_c, #Mode_FIQ:OR:I_Bit:OR:F_Bit
        MOV     sp, r0
; Could set up other stacks for other modes...

; Setup application stack
; stack_base could be defined below, or located in a scatter file
        LDR     r0, stack_base

        MSR     CPSR_c, #Mode_SVC:OR:I_Bit:OR:F_Bit
        MOV     sp, r0


; Leave core in SVC mode


        IMPORT  InitTCMentry [WEAK]         ; Import label to TCM init code, but don't fault if not present
        BL      InitTCMentry                ; Execute function if present

        IMPORT  InitMMUentry [WEAK]         ; Import label to MMU init code, but don't fault if not present
        BL      InitMMUentry                ; Execute function if present


; Branch to C Library entry point

        IMPORT  __main                      ; before MMU enabled import label to __main
        LDR     r12,=__main                 ; save this in register for possible long jump

        BX      r12                         ; branch to __main
        ENDFUNC
        
; --- Location and sizes of stacks
                IF :DEF: LOCATIONS_IN_CODE

stack_base            DCD   0x17000
exception_stack_base  DCD   0x18000

                ELIF :DEF: USE_SCATTER_SYMS

                IMPORT      ||Image$$ARM_LIB_STACKHEAP$$ZI$$Limit||
                IMPORT      ||Image$$EXCEPTION_HANDLER_STACKS$$ZI$$Limit||

stack_base      DCD         ||Image$$ARM_LIB_STACKHEAP$$ZI$$Limit||
exception_stack_base  DCD   ||Image$$EXCEPTION_HANDLER_STACKS$$ZI$$Limit||

                ELSE
                INFO 1, "You need to define LOCATIONS_IN_CODE or USE_SCATTER_SYMS."
                ENDIF

        END
