/*
 * MenuManager.h
 *
 *  Created on: Jun 17, 2009
 *      Author: mbuyanin
 */

#ifndef MENUMANAGER_H_
#define MENUMANAGER_H_

#include "menuCmd.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class MenuMgr : public MenuCmd
{
public:
	MenuMgr(const string name): MenuCmd( name) 			{}
	~MenuMgr() 											{};

	void add(MenuCmd& pCmd) 		{ _menuItemCmd.push_back( &pCmd );}
	bool execute();

private:
	MenuCmd* select(int index) 		{ return _menuItemCmd[index-1];	}
 	void show();
 	int get_user_input();
 	vector<MenuCmd *> _menuItemCmd;
};

#endif /* MENUMANAGER_H_ */
