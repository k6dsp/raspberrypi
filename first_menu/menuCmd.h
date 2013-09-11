/*
 * MenuCmd.h
 *
 *  Created on: Jun 17, 2009
 *      Author: mbuyanin
 */

#ifndef MENUCOMMAND_H_
#define MENUCOMMAND_H_

#include <vector>
#include <string>

using std::string;
using std::vector;

//Abstract.
class MenuCmd
{
public:
  MenuCmd(const std::string name) : _sName( name) {}
  virtual ~MenuCmd() 						     {}
  virtual bool execute() = 0;
  const string get_name() 					 { return _sName; }

private:
  const std::string _sName;
};

//
class QuitCmd : public MenuCmd
{
public:
  QuitCmd() : MenuCmd("Quit Application") 	{}
  bool execute()							{ return false; }
private:

};

//
class ReturnCmd : public MenuCmd
{
public:
  ReturnCmd() : MenuCmd("Return To Previous Menu") 	{}
  bool execute() 									{  return false; };

private:

};

//
class PrintCmd : public MenuCmd
{
public:
	PrintCmd() : MenuCmd("Print") 					{}
	bool execute();
private:

};


#endif /* MENUCOMMAND_H_ */
