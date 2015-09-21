#include "StoryMenu.h"

#include "Button.h"
#include "Slider.h"
#include "Caption.h"

#include "Event.h"
#include "EventMenuControl.h"
#include "EventMenuPop.h"

#include "EventQuit.h"

#include "EventSystem.h"

#include "Game.h"

#include "Display.h"
#include "FontManager.h"

#include "Player.h"
#include "SoundEngine.h"
#include "EntityManager.h"
#include "StickyGrenade.h"

extern Game *game;
extern EventSystem *gpEventSystem;

StoryMenu::StoryMenu() : Menu(game->getDisplay()->getWidth() * 0.75, game->getDisplay()->getHeight() * 0.75)
{
	Button *backButton = New Button(*game->getFontManager()->get("century_gothic"),"Back");

	Caption *titleCaption = New Caption(*game->getFontManager()->get("century_gothic"), "Duke Spookem 3D Story:");
	Caption *storyCaption1 = New Caption(*game->getFontManager()->get("century_gothic"), "Duke Spookem, ghoul exterminator, has been sent\nto town.");

	int div = 15;
	
	storyCaption1->setPosition(Vec2d((getWidth() - storyCaption1->getWidth())/2, 9*getHeight()/div));
	
	addItem("Back", backButton);
	addItem("Title", titleCaption);
	addItem("caption1", storyCaption1);

	setPosition((game->getDisplay()->getDimensions() - getDimensions()) / 2);


	gpEventSystem->addListener(EVENT_MENUCONTROL, this);

	mBackColor = Color(0.1f,0.1f,0.1f,1.0f);
}

void StoryMenu::handleEvent( const Event& theEvent )
{
	switch(theEvent.getType())
	{
		case EVENT_MENUCONTROL:
		{

			MenuItem *control = ((EventMenuControl&)theEvent).getMenuItem();

			if(control == mItems["Back"])
				gpEventSystem->fireEvent(EventMenuPop(this));
		}
		default:
			break;
	}
}