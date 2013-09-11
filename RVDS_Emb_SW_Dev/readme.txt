============================================
Embedded Software Development - Example Code
============================================

This directory contains the example projects referenced in the 
"Developing Software for ARM Processors" Guide,
Chapter 3, "Embedded Software Development"

This directory contains the following sub-directories:

1) dhry
   
   Source files for the Dhrystone benchmarking program.  This program
   provides the code base for the example projects in this directory.

2) source 
   
   All other source files needed to build the example projects.

3) include 
   
   User defined header files.

4) scatter

   Scatter files used to build example projects.

5) build directories

   Batch files to build the example projects.  

The top-level directory also contains an Eclipse project, which includes
targets for each of the example builds.

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

Further details can be found in the readme files of each sub-directory.
