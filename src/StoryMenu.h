#pragma once
#ifndef _STORY_MENU_H
#define _STORY_MENU_H

#include "Menu.h"

class StoryMenu : public Menu
{
	public:
		StoryMenu();

		//Handles incoming events from eventsystem.
		virtual void handleEvent( const Event& theEvent );
};

#endif