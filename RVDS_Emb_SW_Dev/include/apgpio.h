/*
 * Copyright: 
 * ----------------------------------------------------------------
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 *   (C) COPYRIGHT 2003,2004 ARM Limited
 *       ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 * ----------------------------------------------------------------
 * File:     apgpio.h
 * Release:  Version 2.0
 * ----------------------------------------------------------------
 * 
 *  ----------------------------------------
 *  Version and Release Control Information:
 * 
 *  File Name              : $RCSfile: apgpio.h,v $
 *  File Revision          : $Revision: 1.2 $
 * 
 *  ----------------------------------------
 *  
 *            GPIO Interface Support
 *            ======================
 */


// GPIO base addresses
#define GPIO0_BASE      0x101E4000
#define GPIO1_BASE      0x101E5000
#define GPIO2_BASE      0x101E6000
#define GPIO3_BASE      0x101E7000

#define GPIO0_A         (*((volatile unsigned int *)GPIO0_BASE))
#define GPIO1_A         (*((volatile unsigned int *)GPIO1_BASE))
#define GPIO2_A         (*((volatile unsigned int *)GPIO2_BASE))
#define GPIO3_A         (*((volatile unsigned int *)GPIO3_BASE))

// GPIO registers
#define GPIO_DATA       0x3FC       // Data register for all bits
#define GPIO_DIR        0x400       // Data direction register
#define GPIO_IS         0x404       // Interrupt sense
#define GPIO_IBE        0x408       // Interrupt both edges
#define GPIO_IEV        0x40C       // Interrupt event
#define GPIO_IE         0x410       // Interrupt mask
#define GPIO_RIS        0x414       // Interrupt raw status
#define GPIO_MIS        0x418       // Interrupt mask status
#define GPIO_IC         0x41C       // Interrupt clear
#define GPIO_AFSEL      0x420       // Mode control

//GPIO definitions
#define GPIO_REG(slave, offset) *((volatile unsigned int *)((GPIO0_BASE) + (0x1000 * slave) + (offset)))
