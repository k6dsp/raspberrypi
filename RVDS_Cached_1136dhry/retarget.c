/*
** Copyright (C) ARM Limited, 2005-2008. All rights reserved.
*/

#include <stdio.h>
#include <rt_misc.h>

extern void core_init(void);

/*
The functions below are patched onto main.
*/

extern void $Super$$main(void);

void $Sub$$main(void)
{
  core_init();                    // does some extra setup work
  
  $Super$$main();                 // calls the original function
}
