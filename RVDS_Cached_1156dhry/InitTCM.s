;;  Copyright ARM Ltd 2005-2008. All rights reserved.
;;
;;  This code initialises the TCMs before calling __main
;;  this allows scatter loading to relocate code into the TCMs
;;
;;  This code must be run from a privileged mode


Region_Enable  EQU 2_1

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


        AREA   INIT1156TCM, CODE, READONLY

        ENTRY

        EXPORT Start
Start

; if MMU/MPU enabled - disable (useful for ARMulator tests)
;
; Force core into known state - otherwise reload will fail!
;      
       LDR     r0, =0x00450074
       MCR     p15, 0, r0, c1, c0, 0        ; Control reg: Caches, MMU and branch prediction disabled
       MOV     r0, #0x0B
       MCR     p15, 0, r0, c1, c0, 1        ; Aux Control reg.  Individual BP features enabled (overidden in Cntrl Reg)
       
       
       ; Ensure all regions are disabled, for now
       
       MOV    r0, #0x10                      ; Loop counter to step through regions
       MOV    r1, #0x37                      ; For Region size enable reg - 4GB but disabled
       LDR    r2, =0x303                     ; For Access control register, full access, non-shared, cachable write back normal memory
       MOV    r3, #0                         ; For base address, doesn't really matter
       
regiondisableloop
       
       SUBS       r0, r0, #1
       MCR        p15, 0, r0, c6, c2, 0    ; Select region to program
       MCR        p15, 0, r3, c6, c1, 0    ; Base address of zero, for now
       MCR        p15, 0, r1, c6, c1, 2    ; 4GB Disabled
       MCR        p15, 0, r2, c6, c1, 4    ; Access control
       BNE regiondisableloop
       
       MOV     r0, #0
       MCR     p15, 0, r0, c7, c5, 0        ; flush the I Cache
       MCR     p15, 0, r0, c7, c6, 0        ; flush the D Cache
       MCR     p15, 0, r0, c7, c5, 4        ; Flush Prefetch Buffer

       
        
       MCR     p15, 0, r0, c7, c10, 4       ; drain the write buffer
;
;

       IMPORT  ||Image$$ARM_LIB_STACKHEAP$$ZI$$Limit||              ; defined by linker and located by scatter file
       LDR     SP, =||Image$$ARM_LIB_STACKHEAP$$ZI$$Limit||


;
;Next block sets the TCMs base and size registers to their actual size
;Note actual size should be contained in the register at reset.
;
;         
;;TCM Configuration 
;==================

        IMPORT ||Image$$D-TCM$$Base||

        LDR     r0,=||Image$$D-TCM$$Base||  ; Read D-TCM Base - set by scatter file
        MRC     p15, 0, r1, c9, c1, 0       ; Read D-TCM
        AND     r1, r1, #0x7F               ; Clear base address and all SBZ bits
        ORR     r1, r1, r0                  ; Set base address

 IF :DEF: TCM
        ORR     r1, r1, #1                  ; Enable D-TCM
 ELSE
        BIC     r1, r1, #1                  ; Disable I_TCM
 ENDIF
        MCR     p15, 0, r1, c9, c1, 0       ; Set Data D-TCM base
        
;I-TCM base set to 0
 
        MRC     p15, 0, r1, c9, c1, 1       ; Read I-TCM
        AND     r1, r1, #0x7F               ; Clear base address and all SBZ bits

 IF :DEF: TCM
        ORR     r1, r1, #1                  ; Enable I-TCM
 ELSE
        BIC     r1, r1, #1                  ; Disable I_TCM
 ENDIF
        MCR     p15, 0, r1, c9, c1, 1       ; Set I-TCM base
        

;
;
        IMPORT  Init_MPU                    ; Import label to MPU init code

        B       Init_MPU

        END
