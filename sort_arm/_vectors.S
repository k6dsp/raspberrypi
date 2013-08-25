
.globl _start
_start:
    mov sp,#0x8000
    mov r0,pc
    bl main
hang: b hang

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl dummy
dummy:
    bx lr

        .global core_init

core_init:

//==================================================================
// Enable caches
// Caches are controlled by the System Control Register:
//==================================================================
        mrc     p15, 0, r0, c1, c0, 0      // Read CP15 register 1
        orr     r0, r0, #(0x1<<12)         // Enable I Cache
        orr     r0, r0, #(0x1<<2)          // Enable D Cache
        mcr     p15, 0, r0, c1, c0, 0      // Write CP15 register 1

//==================================================================
// Enable Program Flow Prediction
//
// Branch prediction is controlled by the Auxiliary Control Register:
// Set Bit 0 to enable return stack (Set on reset)
// Set Bit 1 to enable dynamic branch prediction (Set on reset)
// Set Bit 2 to enable static branch prediction (Set on reset)
//==================================================================

        mrc     p15, 0, r0, c1, c0, 1      // Read Auxiliary Control Register
        mov     r1, #0x7
        orr     r0 ,r0, r1
        mcr     p15, 0, r0, c1, c0, 1      // Write Auxiliary Control Register

        mrc     p15 ,0, r0, c1, c0, 0      // Read Control Register configuration data
        orr     r0, r0, #(0x1 << 11)       // Enable Program flow prediction
        mcr     p15, 0, r0, c1, c0, 0      // Write Control Register configuration data

        bx     lr

;@-------------------------------------------------------------------------
;@
;@ Copyright (c) 2012 David Welch dwelch@dwelch.com
;@
;@ Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
;@
;@ The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
;@
;@ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;@
;@-------------------------------------------------------------------------
