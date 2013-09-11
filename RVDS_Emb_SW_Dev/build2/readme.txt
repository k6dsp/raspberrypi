Build 2
=======

Build 2 of the example makes use of the Versatile platform's hardware for clocking and string I/O.

The following changes were made to Build 1 of the example project:

· C Library Retargeting

  A retargeted layer of ANSI C functions has been added.  These include 
  standard input/output functionality, clock functionality, as well as 
  some additional error signaling and program exit.

· Target Dependant Device Driver

  A device driver layer that interacts directly with target hardware 
  peripherals has been added.


The symbol  __use_no_semihosting is not imported into this project.  This is 
because a semihosting function is executed during C library initialization to set up the 
application stack and heap location.  

Build Options
-------------
The following predefines are used to control build options.

- __CLK_TCK=1

  __CLK_TCK must be equal to the frequency of the reference clock read by the
  C library clock() function.  In the example project, we use the Versatile
  Real Time Clock (freq = 1Hz).  The default value of __CLK_TCK is 100, 
  because the SYS_CLOCK semihosting operation returns a value in centiseconds 
  (ie. freq = 100Hz).  
  
- USE_TARGET_CLOCK

  Builds with retargeted clocking functionality.  

- USE_SERIAL_PORT

  Builds with retargeted I/O functionality.  
  
- LIGHT_LED

  Enable on-board LED indication of UART port traffic
