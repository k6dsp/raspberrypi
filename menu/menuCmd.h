/*
 * MenuCmd.h
 *
 *  Created on: Jun 17, 2009
 *      Author: mbuyanin
 */

#ifndef MENUCOMMAND_H_
#define MENUCOMMAND_H_

#include <string.h>
extern unsigned int uart_getc ( void );
extern void uart_putl (  char * ptr,  int len );

#define print_uart(x) uart_putl((char *)(x), strlen((x)) )
#define read_uart() uart_getc()

//Abstract.
class MenuCmd
{
public:
    MenuCmd(const char * name) :
        _sName(name)
    {
    }
    //  virtual ~MenuCmd() 						     {}
    virtual bool execute()
    {
        return false;
    }
    ;
    const char * get_name()
    {
        return _sName;
    }

private:
    const char * _sName;
};

//
//
class ReturnCmd: public MenuCmd
{
public:
    ReturnCmd() :
        MenuCmd("Return To Previous Menu")
    {
    }
private:
};

//
class PrintCmd : public MenuCmd
{
public:
	PrintCmd() : MenuCmd("Print") 					{}
	virtual bool execute();
private:

};

class LedOnCmd : public MenuCmd
{
public:
	LedOnCmd() : MenuCmd("Turn On LED") 					{}
	virtual bool execute();
private:

};

class LedOffCmd : public MenuCmd
{
public:
	LedOffCmd() : MenuCmd("Turn Off LED") 					{}
	virtual bool execute();
private:

};

#endif /* MENUCOMMAND_H_ */
