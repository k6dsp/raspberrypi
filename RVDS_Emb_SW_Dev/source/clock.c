/*
** Copyright (C) ARM Limited, 2008. All rights reserved.
*/

/* clock.c */

/*
** This implements a simple clocking mechanism on the Versatile platform
**
** It reads from the Real Time Clock that is present on the
** Versatile AB926 board.
**
** The reference counter ticks at 1Hz.
** this is very poor resolution, but is very simple to implement.
*/

/* 
** Define SCATTER_PERIP if the RTC clock register locations are
** defined in a scatter file
*/

#ifdef SCATTER_PERIP

#include "versatile_struct.h"
//extern struct cm cm0;
extern struct rtc rtc0;
//#define CM_REFCT  cm0.refct
#define RTC_DR    rtc0.DR

#else
#include "versatile.h"
#endif


/* Return the current value from the Versatile reference clock */

unsigned ref_clock(void)
{
    return RTC_DR;
}

void init_clock(void)
{
    RTC_DR=0;        // Zero the Real Time Clock
}

