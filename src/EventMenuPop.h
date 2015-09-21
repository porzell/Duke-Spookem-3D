#pragma once
#ifndef _MENU_POP_EVENT_H

#include "Event.h"
#include "Menu.h"

class EventMenuPop : public Event
{
	public:
		EventMenuPop() : Event( EVENT_MENUPOP ), mPointer(NULL) {}
		EventMenuPop(Menu *menu) : Event( EVENT_MENUPOP ), mPointer(menu) {}
		~EventMenuPop() {}

		inline Menu* getMenu() { return mPointer; }

	private:
		Menu *mPointer;
};

#endif