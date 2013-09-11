;;; Copyright ARM Ltd 1998-2005. All rights reserved.

    PRESERVE8
    AREA SVC_Area, CODE, READONLY

    EXPORT SVC_Handler
    IMPORT C_SVC_Handler

T_bit EQU 0x20

SVC_Handler

    STMFD   sp!, {r0-r3, r12, lr}  ; Store registers
    MOV     r1, sp                 ; Set pointer to parameters
    MRS     r0, spsr               ; Get spsr
    STMFD   sp!, {r0, r3}          ; Store spsr onto stack and another
                                   ; register to maintain 8-byte-aligned stack
    TST     r0, #T_bit             ; Occurred in Thumb state?
    LDRNEH  r0, [lr,#-2]           ; Yes: Load halfword and...
    BICNE   r0, r0, #0xFF00        ; ...extract comment field
    LDREQ   r0, [lr,#-4]           ; No: Load word and...
    BICEQ   r0, r0, #0xFF000000    ; ...extract comment field

        ; r0 now contains SVC number
        ; r1 now contains pointer to stacked registers

    BL      C_SVC_Handler          ; Call main part of handler
    LDMFD   sp!, {r0, r3}          ; Get spsr from stack
    MSR     SPSR_cxsf, r0               ; Restore spsr
    LDMFD   sp!, {r0-r3, r12, pc}^ ; Restore registers and return

    END

