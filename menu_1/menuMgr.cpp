/*
 * MenuMgr.cpp
 *
 *  Created on: Jun 17, 2009
 *      Author: mbuyanin
 */

#include <stdio.h>
#include "menuMgr.h"
#include "util.h"





 extern "C"  void *__dso_handle = (void *)0;

//............................................................................
 extern "C" void *malloc(size_t)
 {
 return (void *)0;
 }
 //............................................................................
 extern "C" void free(void *) {
 }

void *operator new(size_t size) throw() { return malloc(size); }
 //............................................................................
void operator delete(void *p) throw() { free(p); }
 //............................................................................
extern "C" int __aeabi_atexit(void *object,
 void (*destructor)(void *),
 void *dso_handle)
 {
 return 0;
 }


extern "C" void __cxa_pure_virtual() {}

//
bool MenuMgr::execute()
{
  MenuCmd* pCmd;
  do
  {
    show();
    int userInput = get_user_input();
    pCmd = select( userInput );
  }
  while( pCmd->execute() );
  return true;
}

//
void MenuMgr::show()
{
    printf("\r\n*** ");
    printf(get_name());
    printf(" ***\r\n");
    int size = index;

    for (int i = 0; i < size; ++i)
    {
        char tmp_str[2];
        printf(itoa16(i + 1, tmp_str));
        printf(". ");
        printf(_menuItemCmd[i]->get_name());
        printf("\r\n");
    }
}

//
int MenuMgr::get_user_input()
{
    int input = 0;
    char choice;
    while (input < 1 || input > index)
    {
        char rxStr[8];
        uint8_t rxIndx = 0;
        printf("\r\nPlease select an item 1-%d ->", index);
        bool rxStrDone = false;
        while (not rxStrDone)
        {
            itoa(getchar(), &choice);
            printf("%c", choice); //provide echo to the input terminal
            if (choice == '\r')
            {
                printf( "\n");
            }
            if (((choice >= '0') && (choice <= '9')) || (choice == 0x0D) || ((choice >= 'A')
                    && (choice <= 'F')) || ((choice >= 'a') && (choice <= 'f')))
            {
                if (choice == 0x0D)
                {
                    rxStr[rxIndx] = 0;
                    rxStrDone = true;
                }
                else
                {
                    rxStr[rxIndx] = choice;
                    rxIndx++;
                    if (rxIndx == (8 - 1))
                    {
                        rxStr[rxIndx] = 0;
                        rxStrDone = true;
                    }

                }
            }
        }
        atoi16(rxStr, & input);
    }
    return input;
}

