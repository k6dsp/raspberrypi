Dhrystone Benchmarking and MIPs performance
===========================================


Dhrystone
---------

The Dhrystone program by Reinhold Weicker can be used to measure the integer 
processing performance of a system.

There are two different versions of the Dhrystone benchmark commonly quoted:
      Dhrystone 1.1
      Dhrystone 2.1
ARM quotes Dhrystone 2.1 figures.

The Dhrystone performance is calculated from:
      Dhrystones per sec = Processor clock * Number of Runs/Execution time

For the result to be valid, the Dhrystone code must be executed for at least 
two seconds.



Building the Dhrystone code
---------------------------

This directory contains:
- The Dhrystone program in three C files: dhry_1.c, dhry_2.c & dhry.h.
- dhry.bat/dhry.sh (Windows/Unix) which you can use to build Dhrystone on the command-line.
- A make-file (dhry.mk) which you can use to build Dhrystone with a suitable make utility.
- An Eclipse project which you can use to build Dhrystone.

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

Alternatively, you can build Dhrystone directly on the command-line in a single step:

    armcc -W -g -O3 -Otime --no_inline --no_multifile -DMSC_CLOCK dhry_1.c dhry_2.c -o dhry.axf

or (equivalently) as separate compile & link steps:

    armcc -c -W -g -O3 -Otime --no_inline --no_multifile -DMSC_CLOCK dhry_1.c dhry_2.c
    armlink dhry_1.o dhry_2.o -o dhry.axf --info totals

The compiler switches used here are:
'-W' to disable all warnings
'-g' to add debug tables for source-level debugging.
'-O3' to apply maximum optimization.
'-Otime' to give code optimized for speed, rather than (the default) space.
'--no_inline' to disable function inlining, because Dhrystone demands "No procedure merging".
'--no_multifile' to disable multifile compilation, because Dhrystone demands that the two 
source files be compiled independently.
'-DMSC_CLOCK' so the C library function clock() is used for timing measurements.

To build a Thumb version, compile with 'armcc --thumb' instead of 'armcc'.
To build for a specific target processor, add a --cpu option.

Note that dhry_1.c & dhry_2.c contains 'old-style' K&R function declarations, 
which produce warnings when compiled.  The warnings can be disabled with the 
compiler '-W' switch.  


MIPS
----

The MIPS figures ARM (and most of the industry) quotes are "Dhrystone VAX MIPs".
The idea behind this measure is to compare the performance of a machine (in our 
case, an ARM system) against the performance of a reference machine.  The 
industry adopted the VAX 11/780 as the reference 1 MIP machine.  The VAX 11/780 
achieves 1757 Dhrystones per second.

The MIPS figure is calculated by measuring the number of Dhrystones per second 
for the system, and then dividing that figure by the number of Dhrystone per 
second achieved by the reference machine. 

So "80 MIPS" means "80 Dhrystone VAX MIPS", which means 80 times faster than a 
VAX 11/780. 

The reason for comparing against a reference machine is that it avoids the need 
to argue about differences in instruction sets.  RISC processors tend to have 
lots of simple instructions.  CISC machines like x86 and VAX tend to have fewer, 
more complex instructions.  If you just counted the number of instructions per 
second of a machine directly, then machines with simple instructions would get 
higher instructions-per-second results, but may not get the job done any faster. 
By comparing how fast a machine gets a given piece of work done against how fast 
other machines get that work done, the question of the different instruction 
sets is avoided.

