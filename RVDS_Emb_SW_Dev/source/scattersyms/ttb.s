;;; Copyright ARM Ltd 2002-2008. All rights reserved.

        PRESERVE8

        AREA    TableBase, DATA, NOINIT

        EXPORT  TTB

; Create dummy variable used to locate TTB in memory

TTB     SPACE   1

        END

