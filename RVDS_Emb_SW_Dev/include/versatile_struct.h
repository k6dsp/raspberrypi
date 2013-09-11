/******************************************************************************
**                                                                           **
**  Copyright (c) 2002-2008 ARM Limited                                           **
**  All rights reserved                                                      **
**                                                                           **
******************************************************************************/

#ifndef __CMDEF
#define __CMDEF

/*****************************************************************************/
/* The following Structures map to Versatile  register offsets               */
/* Structures are instantiated in separate c files and placed by scatter file*/
/*****************************************************************************/

/* system control registers offset */
struct cm
{ 
  volatile unsigned id;             // @0x00
  volatile unsigned proc;           // @0x04
  volatile unsigned osc;            // @0x08
  volatile unsigned ctrl;           // @0x0C
  volatile unsigned stat;           // @0x10
  volatile unsigned lock;           // @0x14
  volatile unsigned lmbuscnt;       // @0x18
  volatile unsigned auxosc;         // @0x1C
  volatile unsigned sdram;          // @0x20
  volatile unsigned init;           // @0x24
  volatile unsigned refct;          // @0x28
  volatile unsigned unused1;        // @0x2C
  volatile unsigned flags;          // @0x30
  volatile unsigned flagsc;         // @0x34
  volatile unsigned nvflags;        // @0x38
  volatile unsigned nvflagsc;       // @0x3C
  volatile unsigned irq_stat;       // @0x40
  volatile unsigned irq_rstat;      // @0x44
  volatile unsigned irq_enset;      // @0x48
  volatile unsigned irq_enclr;      // @0x4c
  volatile unsigned soft_intset;    // @0x50
  volatile unsigned soft_intclr;    // @0x54
  volatile unsigned space_1;        // @0x58
  volatile unsigned space_2;        // @0x5C
  volatile unsigned fiq_stat;       // @0x60
  volatile unsigned fiq_rstat;      // @0x64
  volatile unsigned fiq_enset;      // @0x68
  volatile unsigned fiq_enclr;      // @0x6C
};

struct irq
{
  volatile unsigned Status;         // @0x00
  volatile unsigned FiqStatus;      // @0x04
  volatile unsigned RawStatus;      // @0x08
  volatile unsigned Select;         // @0x0C
  volatile unsigned Enable;         // @0x10
  volatile unsigned EnableClear;    // @0x14
  volatile unsigned Soft;           // @0x18
};

struct timer
{
  volatile unsigned Load;           // @0x00
  volatile unsigned Value;          // @0x04
  volatile unsigned Control;        // @0x08
  volatile unsigned Clear;          // @0x0C
};

struct uart
{ 
  volatile unsigned dr;             // @0x00
  volatile unsigned ecr;            // @0x04
  volatile unsigned lcrh;           // @0x08
  volatile unsigned lcrm;           // @0x0C
  volatile unsigned lcrl;           // @0x10
  volatile unsigned cr;             // @0x14
  volatile unsigned fr;             // @0x18
  volatile unsigned iir;            // @0x1C
};

struct rtc
{
  volatile unsigned DR;             // @0x00
  volatile unsigned MR;             // @0x04
  volatile unsigned STAT;           // @0x08
  volatile unsigned LR;             // @0x0C
  volatile unsigned CR;             // @0x10
};

#endif
