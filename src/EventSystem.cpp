//Taken from Professor Lawson's example.

#include "EventSystem.h"
#include "Event.h"
#include "EventListener.h"

#include <vector>

EventSystem* gpEventSystem = NULL;

EventSystem::EventSystem()
{
}

EventSystem::~EventSystem()
{
}

void EventSystem::addListener(EventType type, EventListener* pListener)
{
	mListenerMap.insert( pair< EventType, EventListener* >(type, pListener) );
}

void EventSystem::removeListener(EventType type, EventListener *pListener)
{
	pair<multimap<EventType,EventListener*>::iterator,multimap<EventType,EventListener*>::iterator> ret;

	ret = mListenerMap.equal_range( type );
	multimap<EventType,EventListener*>::iterator iter;

	for( iter = ret.first; iter != ret.second; ++iter )
	{
		if( iter->second == pListener )
		{
			mListenerMap.erase( iter );
			break;//to prevent using invalidated iterator
		}
	}
}

void EventSystem::removeListenerFromAllEvents( EventListener* pListener )
{
	multimap<EventType,EventListener*>::iterator iter;

	bool allTheWayThrough = false;

	while( !allTheWayThrough )
	{
		allTheWayThrough = true;
		for( iter = mListenerMap.begin(); iter != mListenerMap.end(); ++iter )
		{
			if( iter->second == pListener )
			{
				mListenerMap.erase( iter );
				allTheWayThrough = false; //didn't make it the whole way through
				break;//to prevent using invalidated iterator
			}
		}
	}
}

void EventSystem::fireEvent( const Event& theEvent )
{
	dispatchAllEvents( theEvent );
}

void EventSystem::dispatchAllEvents( const Event& theEvent )
{
	pair<multimap<EventType,EventListener*>::iterator,multimap<EventType,EventListener*>::iterator> ret;
	ret = mListenerMap.equal_range( theEvent.getType() );

	vector<EventListener*> toBeHandled;

	multimap<EventType,EventListener*>::iterator iter;
	for( iter = ret.first; iter != ret.second; ++iter )
	{
		toBeHandled.push_back(iter->second);
		//iter->second->handleEvent( theEvent );
	}

	for(auto it = toBeHandled.begin(); it != toBeHandled.end(); ++it)
	{
		//try
		//{
			(*it)->handleEvent(theEvent);
		//}
		/*catch(...)
		{

		}*/
	}

}