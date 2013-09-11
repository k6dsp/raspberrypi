;;  Copyright ARM Ltd 2002-2008. All rights reserved.
;;
;;  This code initialises the TCMs before calling MMU Initialisation
;;  this allows scatter loading to relocate code into the TCMs
;;
;;  This code must be run from a privileged mode

    PRESERVE8

; TCM region size definitions

Region_0K      EQU 0
Region_4K      EQU 2_0011
Region_8K      EQU 2_0100
Region_16K     EQU 2_0101
Region_32K     EQU 2_0110
Region_64K     EQU 2_0111
Region_128K    EQU 2_1000
Region_256K    EQU 2_1001
Region_512K    EQU 2_1010
Region_1M      EQU 2_1011

Region_Enable  EQU 2_1

               EXPORT InitTCMentry



        AREA   INIT926TCM, CODE, READONLY   ; name this block of code



InitTCMentry   FUNCTION

; if MMU/MPU enabled - disable (useful for ARMulator tests)
;
;
        MRC     p15, 0, r0, c1, c0, 0       ; read CP15 register 1 into r0
        BIC     r0, r0, #0x1                ; clear bit 0
        MCR     p15, 0, r0, c1, c0, 0       ; write value back

;
; Next block sets the TCMs base and size registers to their actual size
; Note actual size should be contained in the register by default.
;
;
;;TCM Configuration
;==================
;
; Set I and D TCM base and size
;
        LDR     r0,Data_TCM_Base            ; Read D-TCM Base
        MRC     p15, 0, r1, c9, c1, 0       ; Read D-TCM
        ORR     r1, r1, r0

 IF :DEF: TCM                               ; If BuildTCM option, enable TCMs
        ORR     r1, r1, #Region_Enable
 ELSE
        BIC     r1, r1, #Region_Enable      ; otherwise disable TCMs
 ENDIF
        MCR     p15, 0, r1, c9, c1, 0       ; Set Data TCM

        MOV     r0, #0                      ; I-TCM placed at 0x0 in this example
        MRC     p15, 0, r1, c9, c1, 1       ; Read I-TCM
        ORR     r1, r1, r0

 IF :DEF: TCM
        ORR     r1, r1, #Region_Enable
 ELSE
        BIC     r1, r1, #Region_Enable
 ENDIF
        MCR     p15, 0, r1, c9, c1, 1       ; Set I-TCM
;
;
                BX LR
                ENDFUNC

    IF :DEF: LOCATIONS_IN_CODE              ; use hard-coded DTCM location,
Data_TCM_Base   DCD     0x10000
    ELSE                                    ; otherwise...

    IF :DEF: USE_SCATTER_SYMS               ; ...DTCM is placed by scatter file symbols

    IMPORT      ||Image$$D-TCM$$Base||
Data_TCM_Base   DCD     ||Image$$D-TCM$$Base||

    ELSE                                    ; otherwise...
                                            
    IMPORT  dtcm_base                       ; ...DTCM is placed by symbols defined in dtcm.s
Data_TCM_Base   DCD     dtcm_base           ; dtcm_base is exported by dtcm.s
    ENDIF
        ENDIF

        END                                 ; mark the end of this file

