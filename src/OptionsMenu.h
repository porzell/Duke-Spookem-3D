#pragma once
#ifndef _OPTIONS_MENU_H
#define _OPTIONS_MENU_H

#include "Menu.h"

class OptionsMenu : public Menu
{
	public:
		OptionsMenu();

		//Handles incoming events from eventsystem.
		virtual void handleEvent( const Event& theEvent );
};

#endif