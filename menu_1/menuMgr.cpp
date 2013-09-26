/*
 * MenuMgr.cpp
 *
 *  Created on: Jun 17, 2009
 *      Author: mbuyanin
 */

#include <stdio.h>
#include "menuMgr.h"


//............................................................................
 extern "C" void *malloc(size_t) { return (void *) 0; }
 //............................................................................
 extern "C" void free(void *) { }

void *operator new(size_t size) throw() { return malloc(size); }
 //............................................................................
void operator delete(void *p) throw() { free(p); }
 //............................................................................
extern "C" int __aeabi_atexit(void *object,
void (*destructor)(void *),
void *dso_handle) { return 0; }


//extern "C" void __cxa_pure_virtual() {}

//
bool MenuMgr::execute()
{
  MenuCmd* pCmd;
  do
  {
    show();
    int userInput = get_user_input();
    pCmd = select( userInput );
  } while( pCmd->execute() );
  return true;
}

//
void MenuMgr::show()
{
    iprintf("\r\n*** %s ***\r\n", get_name());
    for (int i = 0; i < index; ++i)
    {
        iprintf("%d. %s\r\n", i+1, _menuItemCmd[i]->get_name());
    }
}

//
int MenuMgr::get_user_input()
{
    int input = 0;
    while ((input < 1) || (input > index))
    {
        iprintf("\r\nPlease select an item 1-%d -> ", index);
        if (scanf("%d", &input) == 1)
        {
        	iprintf("%d\r\n", input);
        }
        else
        {
        	iprintf("?");
        }
        fflush(stdin);
    }
    return input;
}

