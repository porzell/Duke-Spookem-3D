//Taken from Professor Lawson's example.

#include "Event.h"

Event::Event(EventType type)
:mType(type)
{
}

Event::~Event()
{
}

const string& Event::getEventName() const
{
	if( mType > EVENT_INVALID && mType < EVENT_MAXIMUM )
	{
		return EVENT_NAMES[mType];
	}
	else
	{
		static const string nullString;
		return nullString;
	}
}
