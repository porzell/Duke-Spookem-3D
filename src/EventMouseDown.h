#pragma once
#ifndef _MOUSEDOWN_EVENT_H
#define _MOUSEDOWN_EVENT_H

#include "Vec2d.h"
#include "Event.h"

class EventMouseDown : public Event
{
public:
	EventMouseDown( const int button, const Vec2d &pos ) : Event( EVENT_MOUSEDOWN ), mPosition(pos), mButton(button) {}
	~EventMouseDown() {}

	inline Vec2d getPosition() { return mPosition; }
	const inline int getButton() { return mButton; }

private:
	Vec2d mPosition;
	int mButton;
};

#endif