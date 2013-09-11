
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

//extern void *(volatile int32_t *) ( unsigned int, unsigned int );
#include "stdint.h"
extern "C" void dummy ( unsigned int );

#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028
#define GPPUD       0x20200094
#define GPPUDCLK0   0x20200098



#define UART0_BASE   0x20201000
#define UART0_DR     (UART0_BASE+0x00)
#define UART0_RSRECR (UART0_BASE+0x04)
#define UART0_FR     (UART0_BASE+0x18)
#define UART0_ILPR   (UART0_BASE+0x20)
#define UART0_IBRD   (UART0_BASE+0x24)
#define UART0_FBRD   (UART0_BASE+0x28)
#define UART0_LCRH   (UART0_BASE+0x2C)
#define UART0_CR     (UART0_BASE+0x30)
#define UART0_IFLS   (UART0_BASE+0x34)
#define UART0_IMSC   (UART0_BASE+0x38)
#define UART0_RIS    (UART0_BASE+0x3C)
#define UART0_MIS    (UART0_BASE+0x40)
#define UART0_ICR    (UART0_BASE+0x44)
#define UART0_DMACR  (UART0_BASE+0x48)
#define UART0_ITCR   (UART0_BASE+0x80)
#define UART0_ITIP   (UART0_BASE+0x84)
#define UART0_ITOP   (UART0_BASE+0x88)
#define UART0_TDR    (UART0_BASE+0x8C)








//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//alt function 5 for uart1
//alt function 0 for uart0

//(3000000 / (16 * 115200) = 1.627
//(0.627*64)+0.5 = 40
//int 1 frac 40

//------------------------------------------------------------------------
void uart_putl (  char * ptr,  int len )
{

    for (int i = 0; i < len; i++)
    {
        while(((*(volatile int *)(UART0_FR)) & 0x20));
        *(volatile int *)UART0_DR = *ptr++;
    }

}
//------------------------------------------------------------------------
unsigned int uart_getc ( void )
{
    while(1)
    {
        if(((*(volatile int32_t *)(UART0_FR)) & 0x10) == 0 ) break;
    }
    return(*(volatile int32_t *)(UART0_DR));
}
//------------------------------------------------------------------------
void uart_init ( void )
{
    unsigned int ra;

    *(volatile int32_t *)(UART0_CR) = 0;

    ra=*(volatile int32_t *)(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=4<<12;    //alt0
    ra&=~(7<<15); //gpio15
    ra|=4<<15;    //alt0
    *(volatile int32_t *)(GPFSEL1) = ra;

    *(volatile int32_t *)(GPPUD) = 0;
    for(ra=0;ra<150;ra++) dummy(ra);
    *(volatile int32_t *)(GPPUDCLK0) = (1<<14)|(1<<15);
    for(ra=0;ra<150;ra++) dummy(ra);
    *(volatile int32_t *)(GPPUDCLK0) = 0;

    *(volatile int32_t *)(UART0_ICR) = 0x7FF;
    *(volatile int32_t *)(UART0_IBRD) = 1;
    *(volatile int32_t *)(UART0_FBRD) = 40;

    *(volatile int32_t *)(UART0_LCRH) = 0x60;//flash fifos
    *(volatile int32_t *)(UART0_LCRH) = 0x70;

    *(volatile int32_t *)(UART0_CR) = 0x301;
}
//------------------------------------------------------------------------
//void hexstrings ( unsigned int d )
//{
//    //unsigned int ra;
//    unsigned int rb;
//    unsigned int rc;
//
//    rb=32;
//    while(1)
//    {
//        rb-=4;
//        rc=(d>>rb)&0xF;
//        if(rc>9) rc+=0x37; else rc+=0x30;
//        uart_putc(rc);
//        if(rb==0) break;
//    }
//    uart_putc(0x20);
//}
////------------------------------------------------------------------------
//void hexstring ( unsigned int d )
//{
//    hexstrings(d);
//    uart_putc(0x0D);
//    uart_putc(0x0A);
//}
//------------------------------------------------------------------------
//int main ( int earlypc, char **argv )
//{
//    unsigned int ra;
//
//    uart_init();
//    hexstring(0x12345678);
//    hexstring(earlypc);
//
//    for(ra=0;ra<30000;ra++)
//    {
//        uart_putc(0x30|(ra&7));
//    }
//
//    for(ra=0;ra<100;ra++) uart_putc(0x55);
//
//    //probably a better way to flush the rx fifo.  depending on if and
//    //which bootloader you used you might have some stuff show up in the
//    //rx fifo.
//    while(1)
//    {
//        if((*(volatile int32_t *)(UART0_FR)) & 0x10) break;
//        *(volatile int32_t *)(UART0_DR);
//    }
//
//    while(1)
//    {
//        ra=uart_getc();
//        if(ra==0x0D) uart_putc(0x0A);
//        uart_putc(ra);
//    }
//return(0);
//}

#include "menuMgr.h"




int main()
{
	uart_init();

	PrintCmd 	printCmd;
	ReturnCmd 	returnCmd;
	print_uart("this is a test\r\n");
  //Setup main menu first. Add submenu's here later.
    MenuMgr mainMenu("Main Menu");

  //Setup a single sub menu.
    MenuMgr fileMenu("File Menu");
  //Add concrete menu's to the sub menu.
    fileMenu.add(printCmd);
    fileMenu.add(returnCmd);
  //And add the sub menu to the main menu.
    mainMenu.add(fileMenu);

  //Run the menu.
    mainMenu.execute();

    return 0;
}





//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

