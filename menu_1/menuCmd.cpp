/*
 * MenuCmd.cpp
 *
 *  Created on: Jun 17, 2009
 *      Author: mbuyanin
 */
#include <stdint.h>
#include <stdio.h>
#include "menuCmd.h"
#include "menuMgr.h"

#define GPSET0  0x2020001C
#define GPCLR0  0x20200028


bool PrintCmd::execute()
{
  iprintf("\r\nI am printing something!\r\n");
  return true;
}

bool LedOffCmd::execute()
{
  *(volatile int32_t *)(GPSET0) = 1<<16;
  return true;
}

bool LedOnCmd::execute()
{
  *(volatile int32_t *)(GPCLR0) = 1<<16;
  return true;
}
