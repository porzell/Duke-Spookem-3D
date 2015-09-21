#include "MenuStack.h"

#include "Event.h"
#include "EventMenuPop.h"
#include "EventMenuPush.h"

#include "EventSystem.h"

extern EventSystem *gpEventSystem;

MenuStack::MenuStack()
{
	mShouldPop = false;
	gpEventSystem->addListener(EVENT_MENUPOP,this);
	gpEventSystem->addListener(EVENT_MENUPUSH,this);
}

MenuStack::~MenuStack()
{
	for(auto it = mStack.begin(); it != mStack.end(); ++it)
	{
		delete (*it);
	}
}

void MenuStack::push(Menu *menu)
{
	//If the menu is not empty, we want to disable the topmost menu so we can put a new one on.
	if(!mStack.empty())
	{
		mStack.back()->setEnabled(false);
	}

	//Slap the new menu right on top of the stack.
	mStack.push_back(menu);
}

void MenuStack::pop()
{
	//We don't want to do anything if the menu is already empty.
	if(mStack.empty())
		return;

	//Delete the old menu pointer.
	delete mStack.back();
	
	//Pop old menu from stack.
	mStack.pop_back();
	
	//If menu still isn't empty, enable the menu that's now at the top.
	if(!mStack.empty())
		mStack.back()->setEnabled(true);
}

void MenuStack::think()
{
	if(mShouldPop)
	{
		pop();
		mShouldPop = false;
	}
}

void MenuStack::draw()
{
	glDisable(GL_ALPHA_TEST);

	for(auto it = mStack.begin(); it != mStack.end(); ++it)
		(*it)->draw();

	glEnable(GL_ALPHA_TEST);
}

Menu* MenuStack::getCurrentMenu()
{
	//may not need to check this but just to be safe...
	//Update: I do, since calling back() with nothing in the vector is apparently undefined behavior.
	if(mStack.empty())
		return NULL;

	//If we have something, return it.
	return mStack.back();
}

void MenuStack::handleEvent( const Event& theEvent )
{
	switch(theEvent.getType())
	{
		case EVENT_MENUPOP:
		{
			mShouldPop = true;
			break;
		}
		case EVENT_MENUPUSH:
		{
			Menu *newMenu = ((EventMenuPush&)theEvent).getMenu();
			push(newMenu);
			break;
		}
		default:
			break;
	}
}