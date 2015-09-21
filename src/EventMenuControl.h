#pragma once
#ifndef _MENU_CONTROL_EVENT_H

#include "Event.h"
#include "MenuItem.h"

class EventMenuControl : public Event
{
	public:
		EventMenuControl(MenuItem *control) : Event( EVENT_MENUCONTROL ), mPointer(control) { mRedrawOnly = false; }
		EventMenuControl(MenuItem *control, bool isRedrawOnly) : Event( EVENT_MENUCONTROL ), mPointer(control), mRedrawOnly(isRedrawOnly) {}
		~EventMenuControl() {}

		inline MenuItem* getMenuItem() { return mPointer; }
		inline bool isRedrawOnly() { return mRedrawOnly; }

	private:
		MenuItem *mPointer;
		bool mRedrawOnly;
};

#endif