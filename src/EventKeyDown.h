#pragma once
#ifndef _KEYDOWN_EVENT_H
#define _KEYDOWN_EVENT_H

#include "Event.h"

class EventKeyDown : public Event
{
public:
	EventKeyDown( const int key ) : Event( EVENT_KEYDOWN ), mKeyCode(key) {}
	~EventKeyDown() {}

	const inline int getKey() { return mKeyCode; }

private:
	int mKeyCode;
};

#endif