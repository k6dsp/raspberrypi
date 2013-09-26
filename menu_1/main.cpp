#include "menuMgr.h"
#include "stdint.h"
extern "C" void dummy ( unsigned int );
extern "C" void _init(void) {}

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


void uart_init ( void )
{
    unsigned int ra;


    ra=*(volatile int32_t *)(GPFSEL1);
    ra&=~(7<<18);
    ra|=1<<18;
    *(volatile int32_t *)(GPFSEL1) = ra;


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

PrintCmd 	printCmd;
LedOnCmd	ledOnCmd;
LedOffCmd	ledOffCmd;
ReturnCmd 	returnCmd;

//Setup main menu first. Add submenu's here later.
MenuMgr mainMenu("Main Menu");

//Setup  sub menu.
MenuMgr fileMenu("File Menu");
MenuMgr ledMenu("LED Menu");


int main()
{
	uart_init();

  //Add concrete menu's to the sub menu.
    fileMenu.add(printCmd);
    fileMenu.add(returnCmd);

//Add concrete menu's to the sub menu.
	ledMenu.add(ledOnCmd);
	ledMenu.add(ledOffCmd);
	ledMenu.add(returnCmd);

  //And add the sub menu to the main menu.
    mainMenu.add(fileMenu);
    mainMenu.add(ledMenu);

  //Run the menu.
    mainMenu.execute();

    return 0;
}





//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

