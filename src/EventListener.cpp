//Taken from Professor Lawson's example.

#include "EventListener.h"
#include "EventSystem.h"

EventListener::EventListener()
{
}

EventListener::~EventListener()
{
	if( gpEventSystem != NULL )
	{
		gpEventSystem->removeListenerFromAllEvents( this );
	}
}
