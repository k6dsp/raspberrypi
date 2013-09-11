Build 5
=======

Build 5 of the example is equivalent to Build 4, but with all target 
memory map information located in the scatter file.

· Scatter File Symbols

  Symbols to locate the peripherals are declared in assembler modules.

  Symbols to locate the stack and heap are derived from the position of
  objects placed in the scatter file.

· Updated Scatter File

  The embedded scatter file from build 4 is updated to locate the stack, 
  heap, peripherals, data TCM and TLB.

  The stack and heap are located using linker symbols. 

Build Options
-------------
The following predefines are used to control build options.

- USE_SCATTER_SYMS

  Causes init.s and __user_initial_stackheap to access linker symbols to
  place the stack and heap.

- LIGHT_LED

  Enable on-board LED indication of UART port traffic

Note: linker will remove the dtcm.o(DTCM) section because it is not used.
See comment in dtcm.s
