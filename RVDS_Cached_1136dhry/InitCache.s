; Copyright ARM Ltd 2005-2008. All rights reserved.

;==================================================================
; This code provides basic global enable for an ARM1136 cache:
; It also enables branch prediction

; This code must be run from a privileged mode
;==================================================================

        AREA   INIT1136CACHE, CODE, READONLY

        EXPORT core_init
        
;==================================================================
; Enable caches
; Set global core configurations
;==================================================================

core_init FUNCTION
        MRC     p15, 0, r0, c1, c0, 0       ; read CP15 register 1
        ORR     r0, r0, #(0x1  <<12)        ; enable I Cache
        ORR     r0, r0, #(0x1  <<2)         ; enable D Cache
        MCR     p15, 0, r0, c1, c0, 0       ; write CP15 register 1

;==================================================================
; Enable Program Flow Prediction
;
; Branch prediction is controlled by the  Auxiliary Control Register:
; It is selected by reading or writing CP15,c1, with Opcode2 field set to 1:
; Set Bit 0 to enable return stack (Set on reset)
; Set Bit 1 to enable dynamic branch prediction (Set on reset)
; Set Bit 2 to enable static branch prediction (Set on reset)
;==================================================================

        MRC       p15,0,r0,c1,c0,1 ; Read Auxiliary Control Register
        MOV       r1, #0x7
        ORR       r0,r0,r1
        MCR       p15,0,r0,c1,c0,1 ; Write Auxiliary Control Register

        ; Turning on branch prediction requires a general enable
        ; CP15, c1. Control Register

        ; Bit 11 [Z] bit Program flow prediction:
        ; 0 = Program flow prediction disabled
        ; 1 = Program flow prediction enabled.

        MRC       p15,0,r0,c1,c0,0 ; Read Control Register configuration data
        ORR       r0, r0, #(0x1  <<11)
        MCR       p15,0,r0,c1,c0,0 ; Write Control Register configuration data

;==================================================================

        BX        lr

        ENDFUNC

        END

