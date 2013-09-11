;==================================================================
; Copyright ARM Ltd 2005-2010. All rights reserved.
;
; ARM1136 Embedded example - Startup Code
;==================================================================

        PRESERVE8
        AREA   INITARM1136, CODE, READONLY

        ENTRY

; Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_USR        EQU     0x10
Mode_FIQ        EQU     0x11
Mode_IRQ        EQU     0x12
Mode_SVC        EQU     0x13
Mode_ABT        EQU     0x17
Mode_UNDEF      EQU     0x1B
Mode_SYS        EQU     0x1F

I_Bit           EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40 ; when F bit is set, FIQ is disabled

        EXPORT  Start
Start

;==================================================================
; Disable MMU in case it was left enabled from an earlier run
; This does not need to be done from a cold reset 
;==================================================================

        MRC     p15, 0, r0, c1, c0, 0       ; read CP15 register 1
        BIC     r0, r0, #0x1                ; clear bit 0
        MCR     p15, 0, r0, c1, c0, 0       ; write value back

;===================================================================        
; Invalidate instruction and data cache, also flushes BTAC
;===================================================================     

        MOV     r0, #0                      ; SBZ
        MCR     p15, 0, r0, c7, c7, 0       ; Invalidate both caches
                                            ; Also flushes BTAC

;==================================================================
; Initialise Supervisor Mode Stack
; Note stack must be 8 byte aligned.
;==================================================================

        IMPORT  ||Image$$ARM_LIB_STACKHEAP$$ZI$$Limit||   ; Get Stack from scatter file
        LDR     SP, =||Image$$ARM_LIB_STACKHEAP$$ZI$$Limit||
        
;==================================================================
; Configure TCMs
; Note TCM actual size is stored in the registers at reset.
; Inst TCM location read from scatter file.
; Data TCM location read from scatter file.
;===================================================================

Region_Enable  EQU 2_1

        IMPORT ||Image$$D-TCM$$Base||       ; from scatter file.

        LDR     r0,=||Image$$D-TCM$$Base||  ; Read D-TCM Base

        ; Configure data TCM - CP15 Data TCM region register c9
        
        MRC     p15, 0, r1, c9, c1, 0       ; Read D-TCM
        ORR     r1, r1, r0                  ; Set D-TCM Base address
        
 IF :DEF: TCM                               ; If TCM defined, enable D-TCM
        ORR     r1, r1, #Region_Enable
 ELSE
        BIC     r1, r1, #Region_Enable      ; otherwise disable TCMs
 ENDIF
        MCR     p15, 0, r1, c9, c1, 0       ; Set Data TCM

        
        MOV     r0,#0                       ; Set I-TCM Base to 0x0
        
        ; Configure instruction TCM - CP15 Instruction TCM region register c9
        
        MRC     p15, 0, r1, c9, c1, 1       ; Read I-TCM
        ORR     r1, r1, r0                  ; Set I-TCM Base address
        
 IF :DEF: TCM
        ORR     r1, r1, #Region_Enable      ; If TCM defined, enable I-TCM
 ELSE
        BIC     r1, r1, #Region_Enable
 ENDIF
        MCR     p15, 0, r1, c9, c1, 1       ; Set I-TCM
        
        
;===================================================================
; MMU Configuration
;
; Configure system to use extended v6 format pagetables
; Set translation table base
;===================================================================


        IMPORT ||Image$$TTB$$ZI$$Base||  ; from scatter file.


        ; Specify v6 format pagetables with no subpages
        ; set bit 23 [XP] in CP15 control register.

        MRC     p15,0,r0,c1,c0,0
        MOV     r1, #0x800000
        ORR     r0,r0,r1
        MCR     p15,0,r0,c1,c0,0

        ; ARM1136 supports two translation tables
        ; Configure translation table base (TTB) control register cp15,c2
        ; to a value of all zeros, indicates we are using TTB register 0.

        MOV     r0,#0x0
        MCR     p15, 0, r0, c2, c0, 2
        
        ; write the address of our page table base to TTB register 0.

        LDR     r0,=||Image$$TTB$$ZI$$Base||
        MCR     p15, 0, r0, c2, c0, 0     
        
         
        
;===================================================================
; PAGE TABLE generation 

; Generate the page tables
; Build a flat translation table for the whole address space.
; ie: Create 4096 1MB sections from 0x000xxxxx to 0xFFFxxxxx


; 31                 20 19  18  17  16 15  14   12 11 10  9  8     5   4    3 2   1 0
; |section base address| 0  0  |nG| S |APX|  TEX  |  AP | P | Domain | XN | C B | 1 0|
;
; Bits[31:20] Top 12 bits of VA is pointer into table
; nG[17]=0.   Non global, enables matching against ASID in the TLB when set.
; S[16]=0.    Indicates normal memory is shared when set.
; Access Permissions - configure for full read/write acess in all modes
; APX[15]=0 and AP[11:10]=11

; Set attributes to normal memory, non cacheable.
; TEX[14:12]=001 and CB[3:2]= 00
; P[9]=0.     ECC enabled memory (not supported on ARM1136).
; Set all pages to use domain 15
; Domain[5:8]=1111
; XN[4]:=0    Execute never disabled.

; Bits[1:0]   Indicate entry is a 1MB section.
;===================================================================

        LDR     r1,=0xfff                   ; loop counter
        LDR     r2,=2_00000000000000000001110111100010

        ; r0 contains the address of the translation table base
        ; r1 is loop counter
        ; r2 is level1 descriptor (bits 19:0)

        ; use loop counter to create 4096 individual table entries.
        ; this writes from address 'Image$$TTB$$ZI$$Base' + 
        ; offset 0x3FFC down to offset 0x0 in word steps (4 bytes)

init_ttb_1

        ORR     r3, r2, r1, LSL#20          ; r3 now contains full level1 descriptor to write
        STR     r3, [r0, r1, LSL#2]         ; str table entry at TTB base + loopcount*4
        SUBS    r1, r1, #1                  ; decrement loop counter
        BPL     init_ttb_1

        ; In this example we will set the cacheable attribute in the first descriptor only
        ; So virtual memory from 0 to 1MB will be cacheable (write back mode).
        ; TEX[14:12]=000 and CB[3:2]= 11

        ORR     r3,r3,#2_1100                    ; set CB bits
        BIC     r3,r3,#2_111000000000000         ; clear TEX bits
        STR     r3,[r0]


;===================================================================        
; Setup domain control register
;===================================================================

        ; Enable all domains to client mode
        MRC     p15, 0, r0, c3, c0, 0     ; Read Domain Access Control Register
        LDR     r0, =0x55555555           ; This initialize every domain entry to b01 (client)
        MCR     p15, 0, r0, c3, c0, 0     ; Write Domain Access Control Register          


;===================================================================
; Enable MMU and Branch to __main
;===================================================================

        IMPORT  __main                      ; before MMU enabled import label to __main
        LDR     r12,=__main                 ; save this in register for possible long jump


        MRC     p15, 0, r0, c1, c0, 0       ; read CP15 register 1
        ORR     r0, r0, #0x1                ; enable MMU before scatter loading
        MCR     p15, 0, r0, c1, c0, 0       ; write CP15 register 1


; Now the MMU is enabled, virtual to physical address translations will occur. This will affect the next
; instruction fetch.
;
; The two instructions currently in the ARM pipeline will have been fetched before the MMU was enabled.
; This property is useful because the next two instructions are safe even if new instruction fetches fail -
; If this routine was mapped out of the new virtual memory map, the branch to __main would still succeed.

        BX      r12                         ; branch to __main  C library entry point

        END
