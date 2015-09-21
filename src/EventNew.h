#pragma once
#ifndef _NEW_EVENT_H
#define _NEW_EVENT_H

#include "Event.h"

class EventNew : public Event
{
public:
	EventNew() : Event(EVENT_NEW) {}
	~EventNew() {}
};

#endif