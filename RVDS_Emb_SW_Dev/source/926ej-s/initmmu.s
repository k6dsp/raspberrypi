;;  Copyright ARM Ltd 2002-2008. All rights reserved.
;;
;;  This code provides basic initialization for an ARM92XT including:
;;
;;  - creation of a 16KB level 1 page table in physical memory (this will probably
;;    need to be changed for your target)
;;  - possible modifying of this table to perform ROM/RAM remapping
;;
;;  This code must be run from a privileged mode

        PRESERVE8

        AREA   InitMMU, CODE, READONLY      ; name this block of code

Fault           EQU     2_00                ; constant defines for level 1 pagetable
Section         EQU     2_0010              ; 2_ denotes a binary number
B               EQU     2_0100
C               EQU     2_1000
TTBit           EQU     2_10000
Domain          EQU     2_111100000
FullAccess      EQU     2_110000000000

    IF :DEF: LOCATIONS_IN_CODE
ttb_first_level DCD     0x28000
    ELSE
        IMPORT  TTB
ttb_first_level DCD     TTB
    ENDIF

        EXPORT InitMMUentry

InitMMUentry    FUNCTION


;
; if MMU/MPU enabled - disable it (useful for ARMulator tests)
; also disable the caches and and invalidate the TLBs
; NOTE: this would not be required from a cold reset
;
        MRC     p15, 0, r0, c1, c0, 0       ; read CP15 register 1 into r0
        BIC     r0, r0, #0x1                ; clear bit 0
        MCR     p15, 0, r0, c1, c0, 0       ; write value back
;
        MOV     r0, #0
        MCR     p15, 0, r0, c7, c7, 0       ; invalidate caches
        MCR     p15, 0, r0, c8, c7, 0       ; invalidate TLBs
;
;
init_ttb
        LDR     r0, ttb_first_level         ; set start of Translation Table base (16k Boundary)
        MCR     p15, 0, r0, c2, c0, 0       ; write to CP15 register 2
;
;
; Create translation table for flat mapping
; Top 12 bits of VA is pointer into table
; Create 4096 entries from 000xxxxx to fffxxxxx
;
;
        LDR     r1,=0xfff                   ; loop counter
        MOV     r2, #TTBit:OR:Section       ; build descriptor pattern in register
        ORR     r2, r2, #Domain:OR:FullAccess
;
init_ttb_1
        ORR     r3, r2, r1, LSL#20          ; use loop counter to create individual table entries
        STR     r3, [r0, r1, LSL#2]         ; str r3 at TTB base + loopcount*4
        SUBS    r1, r1, #1                  ; decrement loop counter
        BPL     init_ttb_1
;
; In this example we will set the cacheable
; and bufferable attribute in the first descriptor only
;
        ORR     r3,r3,#2_1100               ; set cachable and bufferable attributes for section 0 (3:2)
        STR     r3,[r0]                     ; ie lower 1MB - this will set cache to Write Back mode

;
; init_domains
        MOV     r0,#(2_01 << 30)            ; must define behaviour for domain 15 (31:30), set client
        MCR     p15, 0, r0, c3, c0, 0       ; write to CP15 register 5

; enable MMU

        MRC     p15, 0, r0, c1, c0, 0       ; read CP15 register 1 into r0
        BIC     r0, r0, #(0x1  <<12)        ; ensure I Cache disabled
        BIC     r0, r0, #(0x1  <<2)         ; ensure D Cache disabled
        ORR     r0, r0, #0x1                ; enable MMU before scatter loading
        MCR     p15, 0, r0, c1, c0, 0       ; write CP15 register 1
;
; Now the MMU is enabled, virtual to physical address translations will occur and effect the next
; instruction fetch. Even if this module is remapped, the branch instruction should be safe as it is
; contained in the pipeline.  However, this should not be relied upon (as this file stands, it flat-maps
; the entire address space, so there is no problem.

                BX LR
                ENDFUNC


                END                          ; mark the end of this file
