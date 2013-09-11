/*
** Copyright (C) ARM Limited, 2002-2008. All rights reserved.
*/

/*
** $Sub$$main() is executed immediately before the user defined main() function.  
** Compile this with -DUSE_SERIAL_PORT to initialize serial port on Versatile.
** For information on $Sub and $Super, see the Linker and Utilities Guide
*/


__weak extern void cache_init(void);   // only linked-in if present
void __enable_irq(void); // compiler intrinsic
extern void $Super$$main(void);
extern void init_serial_A(void);


void $Sub$$main(void)
{
    cache_init();                    // enables caches
#ifdef USE_SERIAL_PORT
    init_serial_A();                 // initialize the AP serial port A
#endif
    __enable_irq();                  // enable IRQs

    $Super$$main();                  // calls the original function main()
}

