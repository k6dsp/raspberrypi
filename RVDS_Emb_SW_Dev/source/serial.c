/*
** Copyright (C) ARM Limited, 2002-2008. All rights reserved.
*/

/* serial.c */

/*
** This implements a simple (polled) RS232 serial driver for the 
** ARM Versatile Board.
**
** It outputs single characters on Serial Port A at 38400 Baud, 
** 8 bit, no parity, 1 stop bit.
**
** Initialize the port with init_serial_A() before calling sendchar().
**
** To monitor the characters output, use a null-modem cable to
** connect Serial Port A to an RS232 terminal or PC running a
** terminal emulator, e.g. HyperTerminal.
*/

/* 
** Define SCATTER_PERIP if the UART register locations are
** defined in a scatter file
*/

#include "uart.h"

// UART base addresses
//#define UART0_BASE       0x101F1000
#define UART_SPEED 38400
#define UART_CLOCK 24000000

// Reference clocks
//#define UART_REF_CLK012  24000000    // 24MHz
//#define UART_REF_CLK3    24000000    // 24MHz

// UART Interrupt register address bit definitions
#define UART_DSRMIM      0x0008
#define UART_DCDMIM      0x0004
#define UART_CTSMIM      0x0002
#define UART_RIMIM       0x0001

#ifdef SCATTER_PERIP

extern struct uart uart0;
#define UART0_DR   uart0.dr
#define UART0_RSR  uart0.ecr 
#define UART0_FR   uart0.fr
#define UART0_IBRD uart0.ibrd
#define UART0_FBRD uart0.fbrd
#define UART0_LCRH uart0.lcrh
#define UART0_CR   uart0.cr

#else

#include "versatile.h"

#endif

void init_serial_A(void)
{
  //Uncomment the below code in a system where boot monitor does not exist
  /*
  //First, disable everything.
  UART0_CR = 0;  //disable everything before updating CR

  //set the correct baud rate and word length, enable FIFO
  //Note:IBRD and FBRD writes must be performed before the LCRH
  //write as LCRH generates the write strobe to transfer the data
  UART0_IBRD = IBRD_Baud_38400;    //integer divider                                   
  UART0_FBRD = FBRD_Baud_38400;    //fraction divider
  UART0_LCRH = LCRH_Word_Length_8 | LCRH_Fifo_Enabled;  
 
  //enable the UART without interrupts
  UART0_CR  = CR_UART_Enable; 
  */
}


void sendchar(unsigned char *ch)
{
  while (UART0_FR & FR_TX_Fifo_Full)
    ;
  
  if (*ch == '\n')                    // Replace line feed with '\r'
    *ch = '\r';
  UART0_DR = *ch;                     // Transmit next character
}

char receive_char(void)
{
  volatile char status;
  volatile char ch = '0';

  while (UART0_FR & FR_RX_Fifo_Empty) // wait until we receive a byte
    ;
  ch = UART0_DR;         // receive character
  status = UART0_RSR;    // read the status register
  
  /* Could check status receive status here */
  return ch;
}

