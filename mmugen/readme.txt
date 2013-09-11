MMUgen Utility
==============

mmugen is a utility for generating MMU page table entries for ARM cached cores.
The utility processes a text file (known as a rule file) which depicts the 
memory layout of a particular system and creates one or two binary files 
containing the necessary entries for the Level 1 and Level 2 page tables.  
This utility can be used to generate v4,v5,v6, or v7 page table formats.  
For more information about the page table formats or MMU settings for your 
specific core, please refer to your core's TRM.


Conditions of Use
-----------------

mmugen has been adapted by ARM from an original design developed by Peter Maloy 
of CodeSprite, www.codesprite.com

No support will be provided by ARM relating to the use of this product.


Files included
--------------

This package includes an Eclipse project which contains the following source files:
   
main.c            - parses rule file and sets entry bits
level1.c          - generates Level 1 page table entries
level2.c          - generates Level 2 page table entries
missing_region.c  - fills in missing entries from Level 1 or Level 2 tables
checking.c        - prints each page table entry for verification purposes
mmugen.h          - contains global structures and function prototypes

It also includes three example rule files:

simple.rule       - a simple rule file which generates v4/v5 format Level 1 and Level 2 tables  
v6.rule           - a more complex rule file which generates v6 format Level 1 and Level 2 tables
CT1176_EB.rule    - a flat-mapped rule file which depicts the memory map of an ARM Emulation Baseboard fitted with an ARM1176 Core Tile

Using mmugen
------------ 

1) Build mmugen.axf using the batch file (build.bat) or the Eclipse project provided

2) Start RVD, connect to the ARM7TDMI RVISS model, and load mmugen.axf

3) Input the following information when prompted by the semihosting console:

 - The name of the rule file you want to process
 - The name of the Level 1 page table binary file  
 - The name of the file which will contain a copy of the generated page table entries
 - Whether you have a Level 2 table (yes or no)
 - The name of the Level 2 Page Table binary file
 - The base address of the Level 2 page table(s)

Please note that if the rule file is not in the same directory as mmugen.axf, 
you will need to use double backslashes ( \\ ) or two periods and a backslash ( ..\ ) 
when specifying the rule file in the semihosting console (for example:  ...\\v6.rule or ..\v6.rule).

4) Include the generated binary file(s) into your application using INCBIN. 
For details about INCBIN, please refer to the RVCT Assembler Guide. 
 

General rule file text
----------------------

Text                            Definition
****                            **********

BASE_ADDRESS                    address of the start of the page table entries
LEVEL                           which page table entries you are writing
VSIX                            using v6 formatted page tables
VSEVEN                          using v7 formatted page tables
DEFAULT                         defines the default page table entry for missing memory regions in the Level 1 and Level 2 tables
VIRTUAL                         defines the virtual addresses
TO                              used to specify the range of virtual addresses 
PHYSICAL                        defines the physical address
POSTPROCESS                     defines postprocessing information              


Valid rule file text for v6/v7 page tables
------------------------------------------

Memory Type             Level   Supported attributes for type           Definition
***********             *****   *****************************           **********
FAULT                   1                                               creates a fault region for specified addresses          
                
PAGES                   1                                               specified addresses are divided into pages of 4KB or 64KB   
                                EEC                                     memory supports error checking
                                DOMAIN_number                           sets domain number associated with memory region    
                                NOT_SECURE                              the memory region is accessed with Non-Secure rights (ARM1176 & Cortex-A8/A9 only)

SECTION                 1                                               specified addresses are divided into 1MB sections
                                NOT_SECURE                              the memory region is accessed with Non-Secure rights (ARM1176 & Cortex-A8/A9 only)
                                NOT_GLOBAL                              the translation is marked as processor-specific in the TLB
                                SHARED                                  memory shared with other components (processors/peripherals)
                                READWRITE                               readwrite memory
                                READONLY                                readonly memory
                                OUTER                                   setting outer cache attributes to non-cacheable/non-bufferable
                                OUTER_B                                 setting outer cache attributes to non-cacheable/bufferable
                                OUTER_C                                 setting outer cache attributes to cacheable/non-bufferable              
                                OUTER_CB                                setting outer cache attributes to cacheable/bufferable
                                NORMAL
                                DEVICE
                                STRONGLY_ORDERED
                                NO_ACCESS               
                                SVC_READWRITE
                                NO_USR_WRITE
                                FULL_ACCESS
                                EEC                                     memory supports error checking  
                                DOMAIN_number                           sets domain number associated with memory region
                                EXECUTE_NEVER                           memory region is not-executable
                                CACHEABLE                               cacheable memory
                                BUFFERABLE                              bufferable memory   
                
SUPERSECTION            1                                               specified addresses are divided into 16MB sections
                                NOT_SECURE                              the memory region is accessed with Non-Secure rights (ARM1176 & Cortex-A8/A9 only)
                                NOT_GLOBAL                              the translation is marked as processor-specific in the TLB
                                SHARED                                  memory shared with other components (processors/peripherals)
                                READWRITE                               readwrite memory
                                READONLY                                readonly memory
                                OUTER                                   setting outer cache attributes to non-cacheable/non-bufferable
                                OUTER_B                                 setting outer cache attributes to non-cacheable/bufferable
                                OUTER_C                                 setting outer cache attributes to cacheable/non-bufferable              
                                OUTER_CB                                setting outer cache attributes to cacheable/bufferable
                                NORMAL
                                DEVICE
                                STRONGLY_ORDERED
                                NO_ACCESS
                                SVC_READWRITE
                                NO_USR_WRITE
                                FULL_ACCESS
                                EEC                                     memory supports error checking
                                EXECUTE_NEVER                           memory region is not-executable
                                CACHEABLE                               cacheable memory
                                BUFFERABLE                              bufferable memory
            
LARGEPAGES              2                                               specified addresses are divided into 16KB sections  
                                EXECUTE_NEVER                           memory region is not-executable
                                OUTER                                   setting outer cache attributes to non-cacheable/non-bufferable
                                OUTER_B                                 setting outer cache attributes to non-cacheable/bufferable
                                OUTER_C                                 setting outer cache attributes to cacheable/non-bufferable              
                                OUTER_CB                                setting outer cache attributes to cacheable/bufferable
                                NORMAL
                                DEVICE  
                                STRONGLY_ORDERED
                                NOT_GLOBAL                              the translation is marked as processor-specific in the TLB
                                SHARED                                  memory shared with other components (processors/peripherals)
                                READWRITE                               readwrite memory
                                READONLY                                readonly memory
                                NO_ACCESS
                                SVC_READWRITE
                                NO_USR_WRITE
                                FULL_ACCESS
                                CACHEABLE                               cacheable memory
                                BUFFERABLE                              bufferable memory
                
EXTENDED_SMALLPAGES     2                                               specified addresses are divided into 4KB sections
                                NOT_GLOBAL                              the translation is marked as processor-specific in the TLB
                                SHARED                                  memory shared with other components (processors/peripherals)
                                READWRITE                               readwrite memory
                                READONLY                                readonly memory
                                OUTER                                   setting outer cache attributes to non-cacheable/non-bufferable
                                OUTER_B                                 setting outer cache attributes to non-cacheable/bufferable
                                OUTER_C                                 setting outer cache attributes to cacheable/non-bufferable              
                                OUTER_CB                                setting outer cache attributes to cacheable/bufferable
                                NORMAL
                                DEVICE
                                STRONGLY_ORDERED
                                NO_ACCESS
                                SVC_READWRITE
                                NO_USR_WRITE
                                FULL_ACCESS
                                CACHEABLE                               cacheable memory
                                BUFFERABLE                              bufferable memory
                                EXECUTE_NEVER                           memory region is not-executable


Valid rule file text for v4/v5 page tables
------------------------------------------
                

Type                    Level   Supported attributes for type           Definition
****                    *****   *****************************           **********
FAULT                   1                                               creates a fault region for specified addresses          
                
PAGES                   1                                               specified addresses divided into pages  
                                EEC                                     memory supports error checking      
                                DOMAIN_number                           sets domain number associated with memory region    
                
SECTION                 1                                               specified addresses are divided into 1MB sections
                                OUTER                                   setting outer cache attributes to non-cacheable/non-bufferable
                                OUTER_B                                 setting outer cache attributes to non-cacheable/bufferable
                                OUTER_C                                 setting outer cache attributes to cacheable/non-bufferable              
                                OUTER_CB                                setting outer cache attributes to cacheable/bufferable
                                NORMAL
                                DEVICE
                                STRONGLY_ORDERED
                                NO_ACCESS
                                SVC_READWRITE
                                NO_USR_WRITE
                                FULL_ACCESS
                                EEC                                     memory supports error checking  
                                DOMAIN_number                           sets domain number associated with memory region
                                CACHEABLE                               cacheable memory
                                BUFFERABLE                              bufferable memory
                
SUPERSECTION            1                                               specified addresses are divided into 16MB sections
                                OUTER                                   setting outer cache attributes to non-cacheable/non-bufferable
                                OUTER_B                                 setting outer cache attributes to non-cacheable/bufferable
                                OUTER_C                                 setting outer cache attributes to cacheable/non-bufferable              
                                OUTER_CB                                setting outer cache attributes to cacheable/bufferable
                                NORMAL
                                DEVICE
                                STRONGLY_ORDERED
                                NO_ACCESS
                                SVC_READWRITE
                                NO_USR_WRITE
                                FULL_ACCESS
                                EEC                                     memory supports error checking
                                CACHEABLE                               cacheable memory
                                BUFFERABLE                              bufferable memory
            
LARGEPAGES              2                                               specified addresses are divided into 64KB sections
                                OUTER                                   setting outer cache attributes to non-cacheable/non-bufferable
                                OUTER_B                                 setting outer cache attributes to non-cacheable/bufferable
                                OUTER_C                                 setting outer cache attributes to cacheable/non-bufferable              
                                OUTER_CB                                setting outer cache attributes to cacheable/bufferable  
                                NORMAL
                                DEVICE  
                                STRONGLY_ORDERED
                                NO_ACCESS
                                SVC_READWRITE
                                NO_USR_WRITE
                                FULL_ACCESS
                                CACHEABLE                               cacheable memory
                                BUFFERABLE                              bufferable memory

SMALLPAGES              2                                               specified addresses are divided into 4KB pages      
                                NO_ACCESS
                                SVC_READWRITE
                                NO_USR_WRITE
                                FULL_ACCESS
                                CACHEABLE                               cacheable memory
                                BUFFERABLE                              bufferable memory

                
EXTENDED_SMALLPAGES     2                                               specified addresses are divided into 4KB pages
                                OUTER                                   setting outer cache attributes to non-cacheable/non-bufferable
                                OUTER_B                                 setting outer cache attributes to non-cacheable/bufferable
                                OUTER_C                                 setting outer cache attributes to cacheable/non-bufferable              
                                OUTER_CB                                setting outer cache attributes to cacheable/bufferable
                                NORMAL
                                DEVICE
                                STRONGLY_ORDERED
                                NO_ACCESS
                                SVC_READWRITE
                                NO_USR_WRITE
                                FULL_ACCESS
                                CACHEABLE                               cacheable memory
                                BUFFERABLE                              bufferable memory


Important information
---------------------

* Do not use / in your comments

* Each rule entry must end with the type (e.g. FAULT, PAGE, SECTION, SUPERSECTION, LARGEPAGES, SMALLPAGES, EXTENDED_SMALLPAGES)

* If an unsupported attribute is used for a type, it will be ignored

* If an attribute is not listed, its value is set to zero in the page table entry

* Tiny page generation is not supported

* All access control bits (AP bits) in v4/v5 Level 2 page tables will be set to a single value

* Cacheable and bufferable attributes must be set before the memory type is specify in the rule entry (memory type = NORMAL, DEVICE, STRONGLY_ORDERED)

* The Readonly and Readwrite attribute must be set before the access permission attribute (e.g. NO_ACCESS, SVC_READWRITE, NO_USR_WRITE, FULL_ACCESS)

* The first Virtual address in your Level 1 table must be zero

* To specify the virtual address range, you must use the following format in your rule file:

       VIRTUAL beginning_addr TO ending_addr

* The physical address of a memory region must follow PHYSICAL in the rule entry 


Outputted messages
------------------

MMU table generation complete

        A correct binary file has been generated.

Filling missing page table entries with Fault entry 

        No default page table entry is defined for the current Level.  Missing entries will be defined at FAULT.

Filling missing page table entries with Level 1 Default entry

        Missing table entries will be defined using the Level 1 default entry.

Filling missing page table entries with Level 2 Default entry

        Missing table entries will be defined using the Level 2 default entry.

Warning 1: The Level 1 page table is # entries, instead of 4096

        The generated Level 1 page table is not 4096 entries long.

Warning 2: Ignoring Normal memory type, TEX bits have already been set.

        The TEX have already been set by one of the OUTER_xx attributes, so the NORMAL memory type is ignored.

Warning 3: Ignoring Device memory type, TEX bits have already been set.

        The TEX have already been set by one of the OUTER_xx attributes, so the DEVICE memory type is ignored.

Warning 4: Ignoring Strongly Ordered memory type, TEX bits have already been set.

        The TEX have already been set by one of the OUTER_xx attributes, so the STRONGLY_ORDERED memory type is ignored.

Error 1: Couldn't open Rule file

        The rule file specified could not be opened

Error 2: Couldn't open Output file

        The output binary file could not be opened

Error 3: MMU table could not be generated   

        A correct binary file has not been generated.

Error 4: READONLY and NO_ACCESS are not attributes for v7 page tables

        It is invalid to specify a memory region with READONLY and NO_ACCESS attributes in a v7 page table.

Error 5: Bufferable, not cacheable Normal memory is not allowed.

        The input rule file defined a memory region as NORMAL, BUFFERABLE, and NOT CACHEABLE.

Error 6: Device memory cannot be cacheable.

        The input rule file defined a memory region as DEVICE and CACHEABLE. 

Error 7: Strongly Ordered memory cannot be cacheable or bufferable.

        The input rule file defined a memory region as CACHEABLE and/or BUFFERABLE. 

Error 8: Unrecognized word

        The input rule file contains invalid text. 

Error 9: MMU page table level not set

        The rule file does not contain a LEVEL value.

Error 10: Section not defined from v1 to v2
        
        The input rule file does not define a region from address v1 to address v2.

Error 11: Defined address range cannot be divided into 16MB sections.

        The address range is not divisible by 16MB.

Error 12: Defined address range cannot be divided into 1MB sections.

        The address range is not divisible by 1MB.

Error 13: An invalid entry type was entered.

        The input rule file Level 1 description contains an invalid entry type (not FAULT, PAGE, SECTION, SUPERSECTION)

Error 14: Defined address range cannot be divided into 64KB sections.

        The address range is not divisible by 64KB.

Error 15: Defined address range cannot be divided into 4KB sections.

        The address range is not divisible by 4KB.

Error 16: An invalid PAGE type was entered.

        The input rule file Level 1 description contains an invalid entry type (not LARGEPAGES, SMALLPAGES, EXTENDED_SMALLPAGES)

Error 17: Missing address range cannot be divided into 16MB sections.

        The missing memory region cannot be divided into 16MB section(s).

Error 18: Missing address range is not divisible by 1MB.

        The missing memory region cannot be divided into 1MB section(s).

Error 19: Missing address range cannot be divided into 64KB sections.

        The missing memory region cannot be divided into 64KB section(s).

Error 20: Missing address range is not divisible by 4KB.

        The missing memory region cannot be divided into 4KB section(s).

Error 21: Level 2 table was not declared at runtime.

        There is a Level 2 table in the rule file, but the user said there were no Level 2 table when running mmugen.axf in RVD.

