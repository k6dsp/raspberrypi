/******************************************************************************
**                                                                           **
**  Copyright (c) 2000-2008 ARM Limited                                      **
**  All rights reserved                                                      **
**                                                                           **
******************************************************************************/

/******************************************************************************
*                                                                             *
* The following are for the Versatile UART system. These are bits to enable  *
* the functions in the Command Registers and the individual masks for the     *
* returned Status registers                                                   *
* Register Bits - these require to be set to enable the defined functions     *
*                                                                             *
******************************************************************************/

#ifndef __UARTDEF
#define __UARTDEF

/*****************************************************************************/
/* UART Structure maps to register offsets                                   */
/* Structure is instantiated in UART.c and placed by scatter file            */
/*****************************************************************************/

struct uart
{ volatile unsigned dr;   // @0x0
  volatile unsigned ecr;  // @0x4
  volatile unsigned reserved1[4]; //0x08~0x14 reserved
  volatile unsigned fr;   // @0x18
  volatile unsigned reserved2;    //0x1C reserved
  volatile unsigned ilpr; // @0x20
  volatile unsigned ibrd; // @0x24
  volatile unsigned fbrd; // @0x28  
  volatile unsigned lcrh; // @0x2C
  volatile unsigned cr;   // @0x30
  volatile unsigned ifls; // @0x34
  volatile unsigned imsc; // @0x38
  volatile unsigned ris;  // @0x3c
  volatile unsigned mis;  // @0x40
  volatile unsigned icr;  // @0x44
  volatile unsigned unused[1006]; //0x48~0xFFC not used
};

/*****************************************************************************/
/* Received Status Register - RSR                                            */
/*****************************************************************************/
#define RSR_Overrun_Error   0x08
#define RSR_Break_Error     0x04
#define RSR_Parity_Error    0x02
#define RSR_Framing_Error   0x01

/*****************************************************************************/
/* Line Control High Byte Register - LCRH                                    */
/*****************************************************************************/
#define LCRH_Word_Length_8  0x60
#define LCRH_Word_Length_7  0x40
#define LCRH_Word_Length_6  0x20
#define LCRH_Word_Length_5  0x00
#define LCRH_Fifo_Enabled   0x10
#define LCRH_2_Stop_Bits    0x08
#define LCRH_Even_Parity    0x04
#define LCRH_Parity_Enable  0x02
#define LCRH_Send_Break     0x01

/*****************************************************************************/
/* Integer Baud Rate Register - IBRD                                         */
/* This register specifies the integer part of the Baud rate divisor         */
/*****************************************************************************/
#define IBRD_Baud_38400   0x27


/*****************************************************************************/
/* Fractional Baud Rate Register - FBRD                                      */
/* This register specifies the fractional part of the Baud rate divisor      */
/*****************************************************************************/
#define FBRD_Baud_38400   0x04


/*****************************************************************************/
/* Control Register - CR                                                     */
/*****************************************************************************/
#define CR_Loop_Back_En   0x80
#define CR_Timeout_Int_En 0x40
#define CR_TX_Int_Enable  0x20
#define CR_RX_Int_Enable  0x10
#define CR_ModStat_Int_En 0x08
#define CR_UART_Enable    0x01

/*****************************************************************************/
/* Flag Register - FR                                                        */
/*****************************************************************************/
#define FR_TX_Fifo_Empty  0x80
#define FR_RX_Fifo_Full   0x40
#define FR_TX_Fifo_Full   0x20
#define FR_RX_Fifo_Empty  0x10
#define FR_Busy           0x08
#define FR_Carrier_Detect 0x04
#define FR_Set_Ready      0x02
#define FR_Clear_To_Send  0x01

/*****************************************************************************/
/* Interrupt Identification Register - IIR                                   */
/*****************************************************************************/
#define IIR_RX_Time_Out   0x08
#define IIR_TX            0x04
#define IIR_RX            0x02
#define IIR_Modem         0x01

#endif
