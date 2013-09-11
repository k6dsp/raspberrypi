/*
 * MenuMgr.cpp
 *
 *  Created on: Jun 17, 2009
 *      Author: mbuyanin
 */

#include "menuMgr.h"
#include <cstdlib> //size_t
//#include <iostream>
//
//using std::cout;
//using std::endl;
//using std::cin;

 extern "C"  void *__dso_handle = (void *)0;

//............................................................................
 extern "C" void *malloc(size_t) {
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
// cout << endl << "*** " << get_name() << " ***" << endl;
 std::size_t size = _menuItemCmd.size();
 for(std::size_t i=0; i < size; ++i)
 {
//    cout << i+1 << ". " << _menuItemCmd[i]->get_name() << endl;
  }
}


//
int MenuMgr::get_user_input()
{
  unsigned input = 0;
  while(input < 1 || input > ( _menuItemCmd.size() ) )
  {
//    cout << "Please select an item 1-" << (_menuItemCmd.size()) << endl << ">";
//    cin >> input;
  }
  return input;
}



