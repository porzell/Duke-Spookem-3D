#pragma once
#ifndef _QUIT_EVENT_H
#define _QUIT_EVENT_H

#include "Event.h"

class EventQuit : public Event
{
public:
	EventQuit() : Event(EVENT_QUIT) {}
	~EventQuit() {}
};

#endif