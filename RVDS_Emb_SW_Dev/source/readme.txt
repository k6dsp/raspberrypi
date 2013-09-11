======
Source
======

This directory contains all the source code needed to build the example
projects, apart from the source for Dhrystone itself.  

Contents
========

- retarget.c

  This file contains re-implementations of functions whose C library 
  implementations rely on semihosting.  This includes I/O and 
  clocking functionality.

- clock.c + serial.c

  These files implement simple device drivers for the ARM Versatile AB926 board.  

- uish.s + embed_uish.s

  These files contain reimplementations of __user_initial_stackheap 
  (the stack and heap setup routine).  
  
  uish.s places the stack and heap using hard coded parameters.  The 
  locations were chosen to be suitable to the Versatile platform.

  embed_uish.s places the heap at the location of the symbol 
  bottom_of_heap.  This symbol is placed by the scatter file.  This
  implementation is not needed in Build5 of the example, because there
  is no need to reimplement __user_initial_stackheap, as ARM_LIB_STACKHEAP
  is used in scatter file to define stack/heap region.

- vectors.s

  A standard vector table and dummy exception handlers.

- submain.c

  Contains the function $Sub$$main(), which is executed immediately 
  before the user defined main() function.
  
- \scattersyms

  This directory contains source for creating dummy symbols used to locate 
  the peripheral registers, stack and heap, data TCM and translation table.  

- \926ej-s

  These directories contain MMU/TCM/Cache initialization code 
  for the respective cores.
  
