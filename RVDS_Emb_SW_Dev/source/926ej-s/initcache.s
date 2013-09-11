;;  Copyright ARM Ltd 2002-2008. All rights reserved.
;;
;;  This code enables caches on a 92X core
;;
;;  It is called by $Sub$$main(), before the main application is entered
;;
;;  This code must be run from a privileged mode

        PRESERVE8

        AREA   INIT92XCACHE, CODE, READONLY      ; name this block of code

        EXPORT cache_init
cache_init     FUNCTION

;
; Set global core configurations
;===============================
;
        MRC     p15, 0, r0, c1, c0, 0       ; read CP15 register 1 into r0

        ORR     r0, r0, #(0x1  <<12)        ; enable I Cache
        ORR     r0, r0, #(0x1  <<2)         ; enable D Cache

        MCR     p15, 0, r0, c1, c0, 0       ; write CP15 register 1
        MOV     pc, lr
        ENDFUNC
        
        END                                 ; mark the end of this file

