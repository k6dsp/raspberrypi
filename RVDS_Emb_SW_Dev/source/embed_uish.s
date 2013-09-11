;;  Copyright ARM Ltd 2002-2008. All rights reserved.
;;
;;  This implementation of __user_initial_stackheap places the
;;  heap at the location of the symbol bottom_of_heap.  
;;  This symbol is placed by the scatter file.  
;;
;;  It assumes that the application mode stack has been placed
;;  in the reset handler.  
;;
        PRESERVE8

        AREA   UISH, CODE, READONLY      ; name this block of code

    IF :DEF: LOCATIONS_IN_CODE
heap_base       DCD     0x14000
    EXPORT __user_initial_stackheap
__user_initial_stackheap FUNCTION
    LDR   r0,heap_base
    MOV   pc,lr
    ENDFUNC
    
    ELSE
    
    IF :DEF: USE_SCATTER_SYMS            ; no need to reimplement __user_initial_stackheap
                                         ; as ARM_LIB_STACKHEAP is used in scatter file
    ENDIF
    ENDIF
 
    END
        

;; The following is the equivalent implementation of the above in C
;;
;; extern unsigned int bottom_of_heap;    //defined in heap.s
;;
;; __value_in_regs struct __initial_stackheap __user_initial_stackheap(
;;         unsigned R0, unsigned SP, unsigned R2, unsigned SL)
;; {
;;     struct __initial_stackheap config;
;;
;;     config.heap_base = (unsigned int)&bottom_of_heap; // defined in heap.s
;;                                                       // placed by scatterfile
;;     config.stack_base = SP;   // inherit SP from the execution environment
;;
;;     return config;
;; }

