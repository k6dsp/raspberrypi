;;; Copyright ARM Ltd 2002-2008. All rights reserved.

        PRESERVE8

        AREA Vect, CODE, READONLY

; A vector table and dummy exception handlers


; *****************
; Exception Vectors
; *****************

; Note: LDR PC instructions are used here, though branch (B) instructions
; could also be used, unless the ROM is at an address >32MB.

        ENTRY

        LDR     PC, Reset_Addr
        LDR     PC, Undefined_Addr
        LDR     PC, SVC_Addr
        LDR     PC, Prefetch_Addr
        LDR     PC, Abort_Addr
        NOP                             ; Reserved vector
        LDR     PC, IRQ_Addr
        LDR     PC, FIQ_Addr
        
        IMPORT  Reset_Handler           ; In init.s
        IMPORT  inc_clock   [WEAK]      ; In clock_irq.c
        
Reset_Addr      DCD     Reset_Handler
Undefined_Addr  DCD     Undefined_Handler
SVC_Addr        DCD     SVC_Handler
Prefetch_Addr   DCD     Prefetch_Handler
Abort_Addr      DCD     Abort_Handler
IRQ_Addr        DCD     IRQ_Handler
FIQ_Addr        DCD     FIQ_Handler


; ************************
; Exception Handlers
; ************************

; The following dummy handlers do not do anything useful in this example.
; They are set up here for completeness.

Undefined_Handler
        B       Undefined_Handler
SVC_Handler
        B       SVC_Handler     
Prefetch_Handler
        B       Prefetch_Handler
Abort_Handler
        B       Abort_Handler
IRQ_Handler
        B       inc_clock    ; inc_clock is WEAK, so this will branch to inc_clock if it exists...
        B       IRQ_Handler  ; ...else endless loop
FIQ_Handler
        B       FIQ_Handler
        
        END

