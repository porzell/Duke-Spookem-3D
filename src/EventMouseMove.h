#pragma once
#ifndef _MOUSEMOVE_EVENT_H
#define _MOUSEMOVE_EVENT_H

#include "Vec2d.h"
#include "Event.h"

class EventMouseMove : public Event
{
public:
	EventMouseMove( const Vec2d &pos, const Vec2d &delta ) : Event( EVENT_MOUSEMOVE ), mPosition(pos), mDelta(delta) {}
	~EventMouseMove() {}

	inline Vec2d getPosition() { return mPosition; }
	inline Vec2d getDelta() { return mDelta; }

private:
	Vec2d mPosition, mDelta;
};

#endif