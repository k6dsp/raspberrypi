/*
 * Copyright (C) ARM Limited, 1998-2005. All rights reserved.
 */

void C_SVC_Handler( int svc_num, int *regs )
{
    switch( svc_num )
    {
    case    0:
        regs[0] = regs[0] * regs[1];
    break;

    case    1:
        regs[0] = regs[0] + regs[1];
    break;

    case    2:
        regs[0] = (regs[0] * regs[1]) + (regs[2] * regs[3]);
    break;

/*
Re-entrant SVC:
    case    2:
        regs[0] = add_two(multiply_two(regs[0], regs[1]), multiply_two(regs[2], regs[3]));
    break;
*/

    case    3:
    {
        int w, x, y, z;

        w = regs[0];
        x = regs[1];
        y = regs[2];
        z = regs[3];

        regs[0] = w + x + y + z;
        regs[1] = w - x - y - z;
        regs[2] = w * x * y * z;
        regs[3] =(w + x) * (y - z);
    }
    break;

    }
}

