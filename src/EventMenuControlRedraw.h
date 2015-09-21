#pragma once
#ifndef _MENU_CONTROL_REDRAW_EVENT_H

#include "Event.h"
#include "MenuItem.h"

class EventMenuControlRedraw : public Event
{
public:
	EventMenuControlRedraw(MenuItem *control) : Event( EVENT_MENUCONTROL ), mPointer(control) {}
	~EventMenuControlRedraw() {}

	inline MenuItem* getMenuItem() { return mPointer; }

private:
	MenuItem *mPointer;
};

#endif