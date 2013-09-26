
.text
.globl _start
.func _start
.extern _vStackTop
.extern _init
.extern main

_start:
    ldr sp, =__stack_end__

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


/*
   Call C++ constructors (for objects in "global scope")
   ctor loop added by Martin Thomas 4/2005
   based on a Anglia Design example-application for ST ARM
*/

		LDR 	r0, =__ctors_start__
		LDR 	r1, =__ctors_end__
ctor_loop:
		CMP 	r0, r1
		BEQ 	ctor_end

//		LDR 	r2, [r0], #4
//		STMFD 	sp!, {r0-r1}
//		MOV 	lr, pc
//		MOV 	pc, r2
//		LDMFD 	sp!, {r0-r1}
//		B 		ctor_loop

4:
		ldmdb	r1!, {r2}
		blx		r2
		cmp		r0, r1
		blo		4b

ctor_end:





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
