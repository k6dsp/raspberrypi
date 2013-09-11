;;  Copyright ARM Ltd 2005-2008. All rights reserved.
;;
;;  This code provides basic initialization for an ARM1156T2-S including:  
;;  
;;  - creation of memory protection regions
;;  - setting of region attributes
;;
;;  - enabling the Instruction and Data caches and Write buffer
;;  - enabling Memory Protection Unit
;;  - regions must be defined for TCM memory addresses
;;
;;  This code must be run from a privileged mode

;;  MPU region size defines
;;  Smaller than 4K regions not defined here - can be added (down to 32 bytes)

Region_4K      EQU 2_01011
Region_8K      EQU 2_01100
Region_16K     EQU 2_01101
Region_32K     EQU 2_01110
Region_64K     EQU 2_01111
Region_128K    EQU 2_10000
Region_256K    EQU 2_10001
Region_512K    EQU 2_10010
Region_1M      EQU 2_10011
Region_2M      EQU 2_10100
Region_4M      EQU 2_10101
Region_8M      EQU 2_10110
Region_16M     EQU 2_10111
Region_32M     EQU 2_11000
Region_64M     EQU 2_11001
Region_128M    EQU 2_11010
Region_256M    EQU 2_11011
Region_512M    EQU 2_11100
Region_1G      EQU 2_11101
Region_2G      EQU 2_11110
Region_4G      EQU 2_11111

Region_Enable  EQU 2_1



        AREA   InitMPU, CODE, READONLY

        EXPORT Init_MPU

Init_MPU FUNCTION

; MPU region definitions/properties
; =================================
;
; Note each region base address must be a multiple of its size


; Region 1: SRAM, Base = 0,          Size = 256KB, Cached, Buffered, Full access, Executable
; Region 3: IO,   Base = 0x10000000, Size = 256MB, NCNB,             Priv access, Non-Executable
; Region 4: FLASH:Base = 0x24000000, Size = 32MB,  Cached NB,        Read Only,   Executable


; Set up and enable region 1 - SRAM
        MOV     r0, #1                  ;Region Select
        MCR     p15, 0, r0, c6, c2, 0
        
        MOV     r0, #0                  ;Base Address
        MCR     p15, 0, r0, c6, c1, 0
                
        MOV     r0, #(Region_256K << 1) :OR: Region_Enable
        MCR     p15, 0, r0, c6, c1, 2   ;Region Size and enable
        
        LDR     r0, =0x303              ;Access permission and C/B
        MCR     p15, 0, r0, c6, c1, 4           

; Set up and enable region 3 - IO
        MOV     r0, #3                  ;Region Select
        MCR     p15, 0, r0, c6, c2, 0
        
        MOV     r0, #0x10000000         ;Base Address
        MCR     p15, 0, r0, c6, c1, 0
                
        MOV     r0, #(Region_256K << 1) :OR: Region_Enable
        MCR     p15, 0, r0, c6, c1, 2   ;Region Size and enable
        
        LDR     r0, =0x1110             ;Access permission and C/B
        MCR     p15, 0, r0, c6, c1, 4      

; Set up and enable region 4 - FLASH     (Only present on AP motherboard)
        
        MOV     r0, #4                  ;Region Select
        MCR     p15, 0, r0, c6, c2, 0
        
        MOV     r0, #0x24000000         ;Base Address
        MCR     p15, 0, r0, c6, c1, 0
                
        MOV     r0, #(Region_32M << 1) :OR: Region_Enable
        MCR     p15, 0, r0, c6, c1, 2   ;Region Size and enable
        
        LDR     r0, =0x602              ;Access permission and C/B
        MCR     p15, 0, r0, c6, c1, 4      

;
; Set global core configurations
;===============================
;
        MRC     p15, 0, r0, c1, c0, 0       ; read CP15 register 1
        BIC     r0, r0, #(0x1 <<12)         ; ensure I Cache disabled before MPU enable
        BIC     r0, r0, #(0x1 <<2)          ; ensure D Cache disabled before MPU enable
        ORR     r0, r0, #0x1                ; enable MPU bit
        ORR     r0, r0, #(0x1 <<11)         ; enable branch prediction (individual features already set in Aux control)
        ORR     r0, r0, #(0x1 <<22)         ; enable unaligned accesses (assumed enabled by the compiler for v6 code)        
        MCR     p15, 0, r0, c1, c0, 0       ; write cp15 register 1


        IMPORT  __main                      ; import label to __main
        B       __main                      ; branch to C Library entry

        ENDFUNC

        END

