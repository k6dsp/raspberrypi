/*
** Copyright (C) ARM Limited, 2002-2008. All rights reserved.
*/

/* clock_irq.c */

/*
** This implements a simple clocking mechanism on the Versatile platform
**
** It reads from the Real Time Clock that is present on the
** Versatile AB926.
**
** This clock has a resolution of 1 second, so a large number of Dhrystone
** iterations are necessary for reliable results.
** The initialization code sets this clock to zero.
*/

/* 
** Define SCATTER_PERIP if the clock register locations are
** defined in a scatter file
*/
#include <stdio.h>

#ifdef DEBUG
#include "apgpio.h"
#endif

static volatile int clock_counter;
#ifdef SCATTER_PERIP

#include "versatile_struct.h"
extern struct irq irq0;
extern struct timer timer0;
#define IRQEnableClear irq0.EnableClear
#define IRQSelect irq0.Select
#define IRQEnable irq0.Enable
#define Timer0Load timer0.Load
#define Timer0Value timer0.Value
#define Timer0Control timer0.Control
#define Timer0Clear timer0.Clear

#else
#include "versatile.h"
#endif


/* Return the current clock value */
/* In order to obtain a high precision, whilst keeping the performance  */
/* penalty low, IRQs are only occuring at a rate of 100 per second, but */
/* the current timer value is factored in.  */

unsigned ref_clock(void)
{
    int clk, count;
    unsigned long temp;
    clk=clock_counter;
    count=Timer0Value;
    
    if (clk==clock_counter)  // A timer IRQ did not occur between the counter and timer values being read.
    {
      temp = clk*20+(20-count);
      return temp;
    }
    else                     // A timer IRQ did occur between the counter and timer values being read
    {
      temp = (clk+1)*20;
      return temp;
    }
}

void init_clock(void)
{
    clock_counter = 0;
    Timer0Load = 20;            // With /16, sets 1/100 seconds.
    Timer0Control = 0x66;          // Periodic mode, Divide by 16
    Timer0Control |= 0x80;         // enable timer
    IRQSelect = 0;                 // Generate IRQ interrupt
    IRQEnableClear = 0x9FFFFFFF;   // Disable all interrupts in IRQ controller.
    IRQEnable = 0x10;              // Enable Timer1 IRQ (IRQs should still be disabled in the core.

    #ifdef DEBUG
    // Set GPIO0 as output
    GPIO_REG(0, GPIO_DIR) = 0xFF;
    #endif
}

__irq void inc_clock(void)
{
    #ifdef DEBUG
    // Toggle GPIO0
    static char data = 0;
    data = !data;
    GPIO_REG(0, GPIO_DATA) = data;
    #endif

    Timer0Clear = 0;              // Clear the interrupt
    clock_counter++;              // And increment our 1/100 second clock.
}
