;;  Copyright ARM Ltd 2005-2008. All rights reserved.
;;
;;  This code provides basic global enable for an ARM1156T2 cache:
;;
;;  This code must be run from a privileged mode


        AREA   INIT1156CACHE, CODE, READONLY

        EXPORT core_init

core_init FUNCTION

;
; Set global core configurations
;===============================
;
        MRC     p15, 0, r0, c1, c0, 0       ; read CP15 register 1
        ORR     r0, r0, #(0x1  <<12)        ; enable I Cache
        ORR     r0, r0, #(0x1  <<2)         ; enable D Cache
        MCR     p15, 0, r0, c1, c0, 0       ; write CP15 register 1
        
        BX      lr
        
        ENDFUNC

        END

