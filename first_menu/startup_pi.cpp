
#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))


//*****************************************************************************
//
// Forward declaration of the default handlers. These are aliased.
// When the application defines a handler (with the same name), this will
// automatically take precedence over these weak definitions
//
//*****************************************************************************

extern "C" void reset_handler(void); // The reset handler
WEAK void undefined_handler(void); // The NMI handler
WEAK void swi_handler(void); // The hard fault handler
WEAK void prefetch_handler(void); // Reserved
WEAK void data_handler(void); // Reserved
WEAK void unused_handler(void); // Reserved
WEAK void irq_handler(void); // Reserved
WEAK void fiq_handler(void); // ReservedNMI_Handler(void);




//*****************************************************************************
//
// Forward declaration of the specific IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the application
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//
//*****************************************************************************


//*****************************************************************************
//
// The entry point for the application.
// main() is the entry point for newlib based applications
//
//*****************************************************************************
extern int main(void);

//
////*****************************************************************************
////
//// The vector table.  Note that the proper constructs must be placed on this to
//// ensure that it ends up at physical address 0x0000.0000.
////
////*****************************************************************************
//__attribute__ ((used, section(".isr_vector")))
//void (* const g_pfnVectors[])(void) =
//{
//        reset_handler, // The reset handler
//        undefined_handler, // The NMI handler
//        swi_handler, // The hard fault handler
//        prefetch_handler, // Reserved
//        data_handler, // Reserved
//        unused_handler, // Reserved
//        irq_handler, // Reserved
//        fiq_handler, // Reserved
//};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned char _etext;
extern unsigned char _data;
extern unsigned char _edata;
extern unsigned char __bss_end__;
extern unsigned char __bss_start__;
extern unsigned long __init_array_start;
extern unsigned long __init_array_end;

//extern unsigned long _etext;
//extern unsigned long _sidata;
//extern unsigned long _sdata;
//extern unsigned long _edata;
//extern unsigned long _sbss;
//extern unsigned long _ebss;
//extern unsigned long _estack;
//extern unsigned long __ctors_start__;
//extern unsigned long __ctors_end__;
//
//extern unsigned long __dtors_start__;
//extern unsigned long __dtors_end__;


//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
extern "C" void reset_handler(void)
{
//    unsigned char *pulSrc;
    unsigned char *pulDest;

//    //
//    // Copy the data segment initializers from flash to SRAM.
//    //
//    pulSrc = (unsigned char *) &_etext;
//    for (pulDest = (unsigned char *) &_data; pulDest
//            < (unsigned char *) &_edata;)
//    {
//        *pulDest++ = *pulSrc++;
//    }

    //
    // Zero fill the bss segment.
    //
    for (pulDest = (unsigned char *) &__bss_start__; pulDest < (unsigned char *) &__bss_end__;)
    {
        *pulDest++ = 0;
    }

    //
    // Call constructors
    //
    unsigned long *ctors;
    for (ctors = &__init_array_start; ctors < &__init_array_end;)
    {
        ((void(*)(void)) (*ctors++))();
    }

    // Setup clock and power up peripherals needed for start of the system
//    SystemInit();

    //
    // Call the application's entry point.
    // main() is the entry point for newlib based applications
    //
    main();
    reset_handler(); //Something bad happen. Reset system.
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
//void NMI_Handler(void)
//{
//    //
//    // Enter an infinite loop.
//    //
//    while (1)
//    {
//    }
//}
#if 0


// Use the 'naked' attribute so that C stacking is not used.
__attribute__((naked))
void HardFault_Handler(void)
{
    /*
     * Get the appropriate stack pointer, depending on our mode,
     * and use it as the parameter to the C handler. This function
     * will never return
     */

    __asm( ".syntax unified\n"
            "MOVS   R0, #4  \n"
            "MOV    R1, LR  \n"
            "TST    R0, R1  \n"
            "BEQ    _MSP    \n"
            "MRS    R0, PSP \n"
            "B      HardFault_HandlerC      \n"
            "_MSP:  \n"
            "MRS    R0, MSP \n"
            "B      HardFault_HandlerC      \n"
            ".syntax divided\n");
}

/**
 * HardFaultHandler_C:
 * This is called from the HardFault_HandlerAsm with a pointer the Fault stack
 * as the parameter. We can then read the values from the stack and place them
 * into local variables for ease of reading.
 * We then read the various Fault Status and Address Registers to help decode
 * cause of the fault.
 * The function ends with a BKPT instruction to force control back into the debugger
 */
extern "C" void HardFault_HandlerC(unsigned long *hardfault_args)
{
    volatile unsigned long stacked_r0;
    volatile unsigned long stacked_r1;
    volatile unsigned long stacked_r2;
    volatile unsigned long stacked_r3;
    volatile unsigned long stacked_r12;
    volatile unsigned long stacked_lr;
    volatile unsigned long stacked_pc;
    volatile unsigned long stacked_psr;
    volatile unsigned long _CFSR;
    volatile unsigned long _HFSR;
    volatile unsigned long _DFSR;
    volatile unsigned long _AFSR;
    volatile unsigned long _BFAR;
    volatile unsigned long _MMAR;

    stacked_r0 = ((unsigned long) hardfault_args[0]);
    stacked_r1 = ((unsigned long) hardfault_args[1]);
    stacked_r2 = ((unsigned long) hardfault_args[2]);
    stacked_r3 = ((unsigned long) hardfault_args[3]);
    stacked_r12 = ((unsigned long) hardfault_args[4]);
    stacked_lr = ((unsigned long) hardfault_args[5]);
    stacked_pc = ((unsigned long) hardfault_args[6]);
    stacked_psr = ((unsigned long) hardfault_args[7]);

    // Configurable Fault Status Register
    // Consists of MMSR, BFSR and UFSR
    _CFSR = (*((volatile unsigned long *) (0xE000ED28)));

    // Hard Fault Status Register
    _HFSR = (*((volatile unsigned long *) (0xE000ED2C)));

    // Debug Fault Status Register
    _DFSR = (*((volatile unsigned long *) (0xE000ED30)));

    // Auxiliary Fault Status Register
    _AFSR = (*((volatile unsigned long *) (0xE000ED3C)));

    // Read the Fault Address Registers. These may not contain valid values.
    // Check BFARVALID/MMARVALID to see if they are valid values
    // MemManage Fault Address Register
    _MMAR = (*((volatile unsigned long *) (0xE000ED34)));
    // Bus Fault Address Register
    _BFAR = (*((volatile unsigned long *) (0xE000ED38)));

    __asm("BKPT #0\n");
    // Break into the debugger

}


#endif

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
void undefined_handler(void)
{
    //
    // Go into an infinite loop.
    //
    while (1)
    {
    }
}
void swi_handler(void)
{
    //
    // Go into an infinite loop.
    //
    while (1)
    {
    }
}
void prefetch_handler(void)
{
    //
    // Go into an infinite loop.
    //
    while (1)
    {
    }
}
WEAK void data_handler(void)
{
    //
    // Go into an infinite loop.
    //
    while (1)
    {
    }
}
void unused_handler(void)
{
    //
    // Go into an infinite loop.
    //
    while (1)
    {
    }
}
void irq_handler(void)
{
    //
    // Go into an infinite loop.
    //
    while (1)
    {
    }
}
void fiq_handler(void)
{
    //
    // Go into an infinite loop.
    //
    while (1)
    {
    }
}


