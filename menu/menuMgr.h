/*
 * MenuManager.h
 *
 *  Created on: Jun 17, 2009
 *      Author: mbuyanin
 */

#ifndef MENUMANAGER_H_
#define MENUMANAGER_H_

#include "menuCmd.h"


class MenuMgr : public MenuCmd
{
public:
	MenuMgr(const char * name): MenuCmd( name), index(0) 			{}

	void add( MenuCmd& pCmd) { _menuItemCmd[index++] = &pCmd ;}
	virtual bool execute();

private:
	MenuCmd* select(int index) 		{ return _menuItemCmd[index-1];	}
 	void show();
 	int get_user_input();
 	MenuCmd * _menuItemCmd[20];
 	int index;
};

#endif /* MENUMANAGER_H_ */
