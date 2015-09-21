#pragma once
#ifndef _MOUSEUP_EVENT_H
#define _MOUSEUP_EVENT_H

#include "Vec2d.h"
#include "Event.h"

class EventMouseUp : public Event
{
public:
	EventMouseUp( const int button, const Vec2d &pos ) : Event( EVENT_MOUSEUP ), mPosition(pos), mButton(button) {}
	~EventMouseUp() {}

	inline Vec2d getPosition() { return mPosition; }
	const inline int getButton() { return mButton; }

private:
	Vec2d mPosition;
	int mButton;
};

#endif