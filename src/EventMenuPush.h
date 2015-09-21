#pragma once
#ifndef _MENU_PUSH_EVENT_H

#include "Event.h"
#include "Menu.h"

class EventMenuPush : public Event
{
	public:
		EventMenuPush(Menu *menu) : Event( EVENT_MENUPUSH ), mPointer(menu) {}
		~EventMenuPush() {}

		inline Menu* getMenu() { return mPointer; }

	private:
		Menu *mPointer;
};

#endif