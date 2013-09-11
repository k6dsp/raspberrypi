SVC Handling Example
====================

For an overview of the principles of SVC handling, see the "Handling Processor
Exceptions" chapter of the Developer Guide.

This example demonstrates the handling of SVC (formerly SWI) calls.


The files provided are:

main.c    - contains the main() function, which installs the SVC vector in the 
            exception table, then calls some SVCs (0, 1, 2 & 3) via __svc()

ahandle.s - is the top-level SVC handler, written in assembler.  It is entered 
            via the SVC vector of the execption table.  Both ARM & Thumb SVCs 
            are identified and passed to chandle.c for processing.

chandle.c - is a second-level SVC handler, called from ahandle.s.  
            SVCs 0, 1, 2 & 3 execute some simple arithmetic.

swi.h     - contains the definitions of __svc(0), __svc(1), __svc(2) & __svc(3).
            __svc(3) has an associated __value_in_regs structure for returning 
            values in registers.


To build this example, use the supplied makefile, batch file or Eclipse project.
Alternatively, to build from the command-line, use:

armasm -g ahandle.s
armcc -c -g -O1 main.c
armcc -c -g -O1 chandle.c
armlink ahandle.o main.o chandle.o -o svc.axf

This produces an ELF executable file (svc.axf) which can be loaded into an
ARM debugger.

To open the supplied Eclipse projects:
1) Start Eclipse, then specify a workspace by browsing to a directory.
2) Click File -> Import to open the Import dialog. 
3) Expand the "General" tree node then select "Existing Projects into Workspace",
then click Next.
4) Choose "Select root directory" and browse to the Examples directory (e.g.
..\RVDS\Examples\4.1\<build number>\<platform>).  You can select the entire
Examples directory to import all of the example projects, or browse to a specific
subdirectory to import a single project. The Eclipse projects in the chosen
directory will then be shown in the Import dialog.
5) Select the project(s) to open, then click Finish.



Notes for Cached Cores
======================

If you are using a processor with separate instruction and data caches, you 
must ensure that cache coherence problems do not prevent the new contents of 
the vectors from being used.

The data cache (or at least the entries containing the modified vectors) must 
be cleaned to ensure the new vector contents are written to main memory. You 
must then flush the instruction cache to ensure that the new vector contents 
are read from main memory.  

For details of cache clean and flush operations, see the Technical Reference
Manual for your target processor.



Notes for Semihosting with RVI
==============================

This example has been written for running on a software model of a processor,
such as RVISS or ISSM, and you may experience some problems running it on real
hardware.  To enable semihosting, RVI must set a breakpoint, by default on the
SVC entry in the vector table.  However, if a software breakpoint is set, this
may be accidentally overwritten when the SVC handler is installed by the 
program.  This problem can be avoided by changing the address at which RVI 
places the breakpoint.  For details on how to do this and more background 
information please refer to the RVI User Guide.

