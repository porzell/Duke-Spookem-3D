#pragma once
#ifndef _MENU_STACK_H

#include <list>
#include "Menu.h"

#include "EventListener.h"

class MenuStack : public EventListener
{
	private:
		std::list<Menu*> mStack;
		bool mShouldPop;

	public:

		MenuStack();
		~MenuStack();

		//Push a new menu onto the stack.
		void push(Menu *menu);

		//Remove the top menu.
		void pop();

		void think();

		void draw();

		//Return the topmost manipulatable menu.
		Menu* getCurrentMenu();

		//Check if the MenuStack is empty.
		inline const bool empty() { return mStack.empty(); };

		//Handles incoming events from eventsystem.
		virtual void handleEvent( const Event& theEvent );

};

#endif