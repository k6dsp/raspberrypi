Build 3
=======

Build 3 of the example implements scatterloading, and contains a retargeted 
__user_initial_stackheap.

The following modifications were made to build 2 of the example project:

· Scatterloading

  A simple scatter description file is passed to the linker.
  
· Retargeted __user_initial_stackheap

  You have the option of selecting either a one-region or a two-region implementation.  
  The default is one-region.  The two-region implementation can be selected by 
  defining TWO_REGION_MODEL when assembling.
  
· Avoiding C library Semihosting
  The symbol __use_no_semihosting is imported into this build, because there
  are no longer any C library semihosting functions present in the image.


Build Options
-------------
The following predefines are used to control build options.

- STANDALONE

  Defines USE_TARGET_CLOCK and USE_SERIAL_PORT (see readme in Build 2 directory).
  Also causes __use_no_semihosting to be imported.

- LIGHT_LED

  Enable on-board LED indication of UART port traffic
