/*
 * Copyright (C) ARM Limited, 1998-2005. All rights reserved.
 */

__svc(0) int multiply_two(int, int);
__svc(1) int add_two(int, int);
__svc(2) int add_multiply_two(int, int, int, int);

struct four_results
{
    int a;
    int b;
    int c;
    int d;
};

__svc(3) __value_in_regs struct four_results
    many_operations(int, int, int, int);
