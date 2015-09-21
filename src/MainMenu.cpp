#include "MainMenu.h"

#include "Button.h"
#include "Slider.h"
#include "Caption.h"

#include "Event.h"
#include "EventMenuControl.h"
#include "EventMenuPop.h"

#include "MenuStack.h"

#include "StoryMenu.h"

#include "EventQuit.h"
#include "EventNew.h"
#include "EventLoad.h"

#include "EventSystem.h"

#include "Game.h"

#include "Display.h"
#include "FontManager.h"

#include "SoundEngine.h"

extern Game *game;
extern EventSystem *gpEventSystem;

MainMenu::MainMenu() : Menu(game->getDisplay()->getWidth()/3, game->getDisplay()->getHeight()/1.25)
{
	Button *exitButton = New Button(*game->getFontManager()->get("century_gothic"),"Exit Game");

	Button *newButton = New Button(*game->getFontManager()->get("century_gothic"),"New Game");

	Button *loadButton = New Button(*game->getFontManager()->get("century_gothic"),"Load Game");

	if(game->hasGameStarted())
	{
		Button *saveButton = New Button(*game->getFontManager()->get("century_gothic"),"Save Game");
		Button *backButton = New Button(*game->getFontManager()->get("century_gothic"),"Back");

		saveButton->setPosition(Vec2d((getWidth() - saveButton->getWidth())/2, 3*getHeight()/10));

		backButton->setPosition(Vec2d((getWidth() - saveButton->getWidth()) , getHeight()/10));

		addItem("Back", backButton);
		addItem("Save", saveButton);
	}
	else
	{
		/*Button *storyButton = New Button(*game->getFontManager()->get("century_gothic"),"Story");
		storyButton->setPosition(Vec2d((getWidth() - storyButton->getWidth()) , 5*getHeight()/10));
		addItem("Story", storyButton);*/
	}

	Caption *title = New Caption(*game->getFontManager()->get("century_gothic"), "Duke Spookum 3D");

	loadButton->setPosition(Vec2d((getWidth() - loadButton->getWidth())/2, 5*getHeight()/10));

	newButton->setPosition(Vec2d((getWidth() - loadButton->getWidth())/2, 8*getHeight()/10));

	title->setPosition(Vec2d((getWidth() - title->getWidth())/2, 9.5*getHeight()/10));

	addItem("New", newButton);
	addItem("Load", loadButton);
	addItem("Exit", exitButton);

	addItem("Title", title);

	setPosition((game->getDisplay()->getDimensions() - getDimensions()) / 2);


	gpEventSystem->addListener(EVENT_MENUCONTROL, this);

	mBackColor = Color(0.1f,0.1f,0.1f,1.0f);
}

void MainMenu::handleEvent( const Event& theEvent )
{
	switch(theEvent.getType())
	{
		case EVENT_MENUCONTROL:
		{

			MenuItem *control = ((EventMenuControl&)theEvent).getMenuItem();

			if(control == mItems["Exit"])
			{
				gpEventSystem->fireEvent(EventQuit());
			}
			else if(control == mItems["Load"])
			{
				gpEventSystem->fireEvent(EventLoad());
			}
			else if(control == mItems["New"])
			{
				gpEventSystem->fireEvent(EventNew());
			}
			else if(control == mItems["Back"])
			{
				gpEventSystem->fireEvent(EventMenuPop(this));
			}
			else if(control == mItems["Save"])
			{
				game->save();
			}
			else if(control == mItems["Story"])
			{
				//gpEventSystem->fireEvent(EventMenuPop(this));
				game->getMenuStack()->push(New StoryMenu());
			}
		}
		default:
			break;
	}
}