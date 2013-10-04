.text
.globl _start
.func _start
.extern _vStackTop
.extern _init
.extern main


.globl _start
_start:
    ldr pc,reset_handler
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,unused_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler
reset_handler:      .word reset
undefined_handler:  .word hang
swi_handler:        .word hang
prefetch_handler:   .word hang
data_handler:       .word hang
unused_handler:     .word hang
irq_handler:        .word irq
fiq_handler:        .word hang

reset:
    mov r0,#0x8000
    mov r1,#0x0000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}

    ;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD2
    msr cpsr_c,r0
    mov sp,#0x8000

    ;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD1
    msr cpsr_c,r0
    mov sp,#0x4000

    ;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD3
    msr cpsr_c,r0
    ldr sp, =__stack_end__

    ;@ SVC MODE, IRQ ENABLED, FIQ DIS
    ;@mov r0,#0x53
    ;@msr cpsr_c, r0



	ldr	r1,=__bss_start__
	ldr r2,=__bss_end__
	mov	r3, #0
	/* clear .bss */
clear_bss:
	cmp	r1,r2
	strlo r3, [r1], #4
	blo clear_bss

	ldr	r1,=__stack_start__
	ldr	r2,=__stack_end__
	ldr	r3, StackFillValue
fill_stack:
	cmp	r1,r2
	strlo r3, [r1], #4
	blo fill_stack

	ldr r12, =__libc_init_array
	mov lr, pc
	bx r12



// Call main program: main(0)
// --------------------------
        mov   r0,#0                     // no arguments (argc = 0)
        mov   r1,r0
        mov   r2,r0
        mov   fp,r0                     // null frame pointer
        mov   r7,r0                     // null frame pointer for thumb
        ldr   r10,=main
        mov   lr,pc
        bx    r10                       // enter main()
hang: 	b 	  hang
/* "global object"-dtors are never called and it should not be
   needed since there is no OS to exit to. */

.size   _start, . - _start
.endfunc


StackFillValue: .word 0xDEADC0DE
.globl dummy
dummy:
    bx lr

.globl enable_irq
enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

irq:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_irq_handler
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4



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
