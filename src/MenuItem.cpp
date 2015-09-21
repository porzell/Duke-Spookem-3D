#include "MenuItem.h"

#include "EventSystem.h"
#include "EventMenuControl.h"

extern EventSystem *gpEventSystem;


MenuItem::MenuItem()
{
	mEnabled = true;
}

MenuItem::~MenuItem()
{

}

void MenuItem::draw()
{

}

void MenuItem::trigger()
{
	gpEventSystem->fireEvent(EventMenuControl(this));
}

void MenuItem::requestRedraw()
{
	gpEventSystem->fireEvent(EventMenuControl(this, true));
}

void MenuItem::handleEvent( const Event& theEvent )
{

}