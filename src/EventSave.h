#pragma once
#ifndef _LOAD_EVENT_H
#define _LOAD_EVENT_H

#include "Event.h"

class EventSave : public Event
{
public:
	EventSave() : Event(EVENT_SAVE) {}
	~EventSave() {}
};

#endif