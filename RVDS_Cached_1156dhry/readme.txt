Semihosted version of Dhrystone for the ARM1156T2-S.

At reset, both TCMs have a base address of 0x0.  The scatter file is used to locate 
the stack, heap and D-TCM.  The I-TCM address remains at 0x0.

This code will run on ARMulator.

Note the scatter file assumes a TCM size of 32 KB

InitTCM.s   - entry point for this image
            - locates, configures and enables the TCMs
            - calls InitMPU

InitMPU.s   - creates MPU regions and enables the MPU (prior to scatter loading)
            - caches and regions are invalidated here
            - cache global enables are set to 0x0 before MPU is enabled
            - calls __main

InitCache.s - enables the caches

retarget.c  - patches main to enable the caches.

The example may be built from the supplied makefile or batch files.

Two build options are provided:

buildTCM.bat - enables the TCMs and relocates all code and data into the TCMs
             - This option gives maximum performance and demonstrates the use
               of TCMs and the placing of objects into TCMs by scatterloading

build.bat    - TCMs not enabled - instruction and data caches are enabled

