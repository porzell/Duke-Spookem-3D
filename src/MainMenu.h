#pragma once
#ifndef _MAIN_MENU_H
#define _MAIN_MENU_H

#include "Menu.h"

class MainMenu : public Menu
{
	public:
		MainMenu();

		//Handles incoming events from eventsystem.
		virtual void handleEvent( const Event& theEvent );
};

#endif