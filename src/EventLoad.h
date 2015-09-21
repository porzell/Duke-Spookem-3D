#pragma once
#ifndef _LOAD_EVENT_H
#define _LOAD_EVENT_H

#include "Event.h"

class EventLoad : public Event
{
public:
	EventLoad() : Event(EVENT_LOAD) {}
	~EventLoad() {}
};

#endif