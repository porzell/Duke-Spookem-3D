//Taken from Professor Lawson's example.

#pragma once

#include <string>
#include "Trackable.h"

using namespace std;

enum EventType
{
	EVENT_INVALID = -1,
	EVENT_QUIT,
	EVENT_NEW,
	EVENT_LOAD,
	EVENT_SAVE,
	EVENT_REMOVE,
	EVENT_PAUSE,
	EVENT_MOUSEDOWN,
	EVENT_MOUSEMOVE,
	EVENT_MOUSEUP,
	EVENT_KEYDOWN,
	EVENT_KEYUP,
	EVENT_MENUCONTROL,
	EVENT_MENUPOP,
	EVENT_MENUPUSH,
	EVENT_GAMESTART,
	EVENT_GAMELOSE,
	EVENT_MAXIMUM
};

const string EVENT_NAMES[EVENT_MAXIMUM] =	{	"Quit Event",
												"New Game Event",
												"Load Game Event",
												"Save Game Event"
												"Remove Event",
												"Pause Event",
												"Mouse Down",
												"Mouse Up",
												"Key Down",
												"Key Up",
												"Menu Control Activated",
												"Menu Pop",
												"Menu Push",
												"Start Game",
												"Lose Game"
											};

class Event:public Trackable
{
public:
	Event( EventType type );
	virtual ~Event();

	EventType getType() const { return mType; };
	const string& getEventName() const;

private:
	EventType mType;

};