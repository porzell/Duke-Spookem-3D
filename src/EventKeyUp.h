#pragma once
#ifndef _KEYUP_EVENT_H
#define _KEYUP_EVENT_H

#include "Event.h"

class EventKeyUp : public Event
{
public:
	EventKeyUp( const int key ) : Event( EVENT_KEYUP ), mKeyCode(key) {}
	~EventKeyUp() {}

	const inline int getKey() { return mKeyCode; }

private:
	int mKeyCode;
};

#endif