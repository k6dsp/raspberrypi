;;; Copyright ARM Ltd 2002-2008. All rights reserved.

        PRESERVE8

        AREA   DTCM, DATA, NOINIT

        EXPORT dtcm_base

; Create dummy variable used to locate Data_TCM in memory
; The 'dtcm_base' will be imported by inittcm.s if necessary, see inittcm.s
; Note that if the exported dtcm_base is not used by inittcm.s,
; this section will be removed by the linker.

dtcm_base    SPACE   1

        END

