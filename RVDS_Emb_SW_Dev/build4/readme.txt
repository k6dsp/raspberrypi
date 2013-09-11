Build 4
=======

Build 4 of the example can be run standalone on the Versatile platform.

The following modifications have been made to build 3 of the example project:

· Vector Table

  A vector table has been added to the project, and placed by the scatter file.
  
· Reset Handler

  The reset handler is coded in init.s.  This makes WEAK calls to two separate
  modules, responsible for TCM and MMU setup respectively.  Only ARM926EJ-S
  versions of TCM and MMU set up are provided.  These are WEAK calls, which
  can be simply omitted if not required.  As this does not attempt to
  access any processor specific features, this will run on an Versatile system
  with any core.  Cache enabled versions for other cores can be easily created
  from example code provided in the cached_dhry directory.

  ROM/RAM remapping occurs immediately after reset.

· $Sub$$main( )

  Caches are enabled in $Sub$$main() before entering the main application.
  Again, this is a WEAK call.

· Embedded Scatter File

  An embedded scatter file is used, which reflects the post-remapping view of 
  memory.

· A precise clock is implemented, using a timer to generate an IRQ that increments
  a counter every 1/100 second.

Build Options
-------------
The following predefines are used to control build options.

- LOCATIONS_IN_CODE

  If true, the locations of peripheral registers, stack and heap, data TCM
  and TLB are hard coded in source.  If LOCATIONS_IN_CODE is not defined, 
  they are located by the scatter file (see build 5).  
  
- USE_SERIAL_PORT

  If defined, submain.c calls the initialization routine for the Versatile 
  Serial port.

- LIGHT_LED

  Enable on-board LED indication of UART port traffic
