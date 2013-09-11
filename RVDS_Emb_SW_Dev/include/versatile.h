/******************************************************************************
**                                                                           **
**  Copyright (c) 2000 ARM Limited                                           **
**  All rights reserved                                                      **
**                                                                           **
******************************************************************************/

#ifndef VERSATILE_H
#define VERSATILE_H

/*****************************************************************************/
/*  Base addresses for standard memory-mapped peripherals                    */
/*****************************************************************************/
#define IntBase         0x10140000  /* Interrupt Controller Base             */
#define FIQBase         0x10140004  /* FIQ Controller Base                   */
#define TimerBase       0x101E2000  /* Counter/Timer Base                    */
#define SYSBase         0x10000000  /* System Registers                 */
#define UART0Base       0x101F1000  /* UART0 Registers                       */
#define RTCBase         0x101E8000  /* Real Time Clock Registers             */

/*****************************************************************************/
/*  Definitions of IRQ and FIQ ids                                           */
/*****************************************************************************/
#define SOFTINT         0x000001
#define UARTINT0        0x000002
#define UARTINT1        0x000004
#define KBDINT          0x000008
#define MOUSEINT        0x000010
#define TIMERINT0       0x000020
#define TIMERINT1       0x000040
#define TIMERINT2       0x000080
#define RTCINT          0x000100
#define EXPINT0         0x000200
#define EXPINT1         0x000400
#define EXPINT2         0x000800
#define EXPINT3         0x001000
#define PCIINT0         0x002000
#define PCIINT1         0x004000
#define PCIINT2         0x008000
#define PCIINT3         0x010000
#define LINT            0x020000
#define DEGINT          0x040000
#define ENUMINT         0x080000
#define PCIINT          0x100000
#define APCIINT         0x200000

/*****************************************************************************/
/*  Retain previous definitions from PID7T board for code compatibility      */
/*****************************************************************************/
#define IRQSerialA      UARTINT0
#define IRQSerialB      UARTINT1
#define IRQTimer0       TIMERINT0
#define IRQTimer1       TIMERINT1
#define IRQTimer2       TIMERINT2

/*****************************************************************************/
/* RPS interrupt Controller IRQ register                                     */
/*****************************************************************************/
#define IRQStatus      *((volatile unsigned *)IntBase)
#define IRQRawStatus   *((volatile unsigned *)(IntBase + 0x08))
#define IRQSelect      *((volatile unsigned *)(IntBase + 0x0C))
#define IRQEnable      *((volatile unsigned *)(IntBase + 0x10))
#define IRQEnableClear *((volatile unsigned *)(IntBase + 0x14))
#define IRQSoft        *((volatile unsigned *)(IntBase + 0x18))

/*****************************************************************************/
/* RPS interrupt Controller FIQ register                                     */
/*****************************************************************************/
#define FIQStatus      *((volatile unsigned *)FIQBase)
#define FIQRawStatus   *((volatile unsigned *)(FIQBase + 0x04))
#define FIQSelect      *((volatile unsigned *)(FIQBase + 0x08))
#define FIQEnable      *((volatile unsigned *)(FIQBase + 0x0C))
#define FIQEnableClear *((volatile unsigned *)(FIQBase + 0x10))

/*****************************************************************************/
/*  Counter/timer registers                                                  */
/*****************************************************************************/
#define Timer0Load       *((volatile unsigned *)TimerBase)
#define Timer0Value      *((volatile unsigned *)(TimerBase + 0x04))
#define Timer0Control    *((volatile unsigned *)(TimerBase + 0x08))
#define Timer0Clear      *((volatile unsigned *)(TimerBase + 0x0C))

#define Timer1Load       *((volatile unsigned *)(TimerBase + 0x20))
#define Timer1Value      *((volatile unsigned *)(TimerBase + 0x24))
#define Timer1Control    *((volatile unsigned *)(TimerBase + 0x28))
#define Timer1Clear      *((volatile unsigned *)(TimerBase + 0x2C))


/*****************************************************************************/
/* Counter/Timer control register bits                                       */
/*****************************************************************************/
#define TimerEnable      0x80
#define TimerPeriodic    0x40
#define TimerPrescale0   0x00
#define TimerPrescale4   0x04
#define TimerPrescale8   0x08


/*****************************************************************************/
/*  UART Control Registers (PL011)                                           */
/*****************************************************************************/

#define UART0_DR   *((volatile unsigned *)(UART0Base))
#define UART0_RSR  *((volatile unsigned *)(UART0Base + 0x04))
#define UART0_ECR  *((volatile unsigned *)(UART0Base + 0x04))
#define UART0_IBRD *((volatile unsigned *)(UART0Base + 0x24))
#define UART0_FBRD *((volatile unsigned *)(UART0Base + 0x28))
#define UART0_LCRH *((volatile unsigned *)(UART0Base + 0x2C))
#define UART0_CR   *((volatile unsigned *)(UART0Base + 0x30))
#define UART0_FR   *((volatile unsigned *)(UART0Base + 0x18))
#define UART0_ICR  *((volatile unsigned *)(UART0Base + 0x44))

/*****************************************************************************/
/*  RTC (Real Time Clock) Control Registers (PL031)                          */
/*****************************************************************************/

#define RTC_DR     *((volatile unsigned *)RTCBase)           //Data register
#define RTC_MR     *((volatile unsigned *)(RTCBase + 0x04))  //Match register
#define RTC_LR     *((volatile unsigned *)(RTCBase + 0x08))  //Load register
#define RTC_CR     *((volatile unsigned *)(RTCBase + 0x0C))  //Control register
#define RTC_IMSC   *((volatile unsigned *)(RTCBase + 0x10))  //Interrupt mask set and clear register
#define RTC_RIS    *((volatile unsigned *)(RTCBase + 0x14))  //Raw interrupt status register
#define RTC_MIS    *((volatile unsigned *)(RTCBase + 0x18))  //Masked interrupt status register
#define RTC_ICR    *((volatile unsigned *)(RTCBase + 0x1C))  //Interrupt clear register

#endif
