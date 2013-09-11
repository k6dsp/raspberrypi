Sorts Example
=============

This directory presents a Sorts example, which compares and contrasts an 
'insertion sort', 'shell sort', and 'quick sort' (the built-in C library sort).

To build this example, use the makefile or batch file provided, or the supplied
Eclipse project.

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

Alternatively, to build from the command-line, use:

armcc -c -g -O1 sorts.c
armlink sorts.o -o sorts.axf

This produces an ELF executable file (sorts.axf) which can be loaded into an 
ARM debugger.

The code as supplied does not reset the random seed. This gives reproducible
results on subsequent runs. To obtain different random values for each run,
uncomment the call to srand on line 66.
