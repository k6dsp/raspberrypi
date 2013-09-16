/*
 * MenuMgr.cpp
 *
 *  Created on: Jun 17, 2009
 *      Author: mbuyanin
 */

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
    print_uart("\r\n*** ");
    print_uart(get_name());
    print_uart(" ***\r\n");
    int size = index;

    for (int i = 0; i < size; ++i)
    {
        char tmp_str[2];
        print_uart(itoa16(i + 1, tmp_str));
        print_uart(". ");
        print_uart(_menuItemCmd[i]->get_name());
        print_uart("\r\n");
    }
}

//
int MenuMgr::get_user_input()
{
    int input = 0;
    while (input < 1 || input > index)
    {
        char rxStr[8];
        uint8_t rxIndx = 0;
        print_uart("\r\nPlease select an item 1-");
        print_uart(itoa16(index, rxStr));
        print_uart(" -> ");
        //    cin >> input;
        bool rxStrDone = false;
        while (not rxStrDone)
        {
            char ch[2];
            ch[1] = 0;
            ch[0] = read_uart();
            print_uart( ch); //provide echo to the input terminal
            if (ch[0] == '\r')
            {
                print_uart( "\n");
            }
            if (((ch[0] >= '0') && (ch[0] <= '9')) || (ch[0] == 0x0D) || ((ch[0] >= 'A')
                    && (ch[0] <= 'F')) || ((ch[0] >= 'a') && (ch[0] <= 'f')))
            {
                if (ch[0] == 0x0D)
                {
                    rxStr[rxIndx] = 0;
                    rxStrDone = true;
                }
                else
                {
                    rxStr[rxIndx] = ch[0];
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
    //Enable UART interrup
    //Clean up the buffer
    //Wait for string to be ready
    //Disable UART interrupt
    //Assign input if it is valid
    //~ input = 1;
    return input;
}

