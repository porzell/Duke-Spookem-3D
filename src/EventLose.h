#pragma once
#ifndef _LOSE_EVENT_H
#define _LOSE_EVENT_H

#include "Event.h"

class EventLose : public Event
{
public:
	EventLose() : Event(EVENT_GAMELOSE) {}
	~EventLose() {}
};

#endif