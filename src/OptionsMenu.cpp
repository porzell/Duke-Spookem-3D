#include "OptionsMenu.h"

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
#include "TextureManager.h"
#include "SoundEngine.h"
#include "EntityManager.h"
#include "StickyGrenade.h"

extern Game *game;
extern EventSystem *gpEventSystem;

OptionsMenu::OptionsMenu() : Menu(game->getDisplay()->getWidth()/3, game->getDisplay()->getHeight()/1.25, game->getTextureManager()->get("dukespookem"))
{
	Button *backButton = New Button(*game->getFontManager()->get("century_gothic"),"Back");
	
	Button *toggleGibsButton = New Button(*game->getFontManager()->get("century_gothic"),"Toggle Gibs");
	//Button *toggleGibStreaksButton = New Button(*game->getFontManager()->get("century_gothic"),"Toggle Gib Streaks");
	//Button *toggleMusicButton = New Button(*game->getFontManager()->get("century_gothic"),"Toggle Music");
	Slider *musicVolumeSlider = New Slider(0.8f * getWidth(), Vec2d(0,1), true);
	Slider *voiceVolumeSlider = New Slider(0.8f * getWidth(), Vec2d(0,1), true);
	Slider *difficultySlider = New Slider(0.8f * getWidth(), Vec2d(0,2), true);
	Caption *difficultyCaption = New Caption(*game->getFontManager()->get("century_gothic"), "Hardness");
	Caption *toggleGibsCaption = New Caption(*game->getFontManager()->get("century_gothic"), game->getAllowGibs() ? "Gibs: Lots of Gibs" : "Gibs: No Gibs");
	Caption *dukeQuipsVolumeCaption = New Caption(*game->getFontManager()->get("century_gothic"), "Duke Quips Volume: " + std::to_string(int(game->getPlayer()->getVoiceVolume() * 100)) + "%");
	//Caption *musicVolumeCaption = New Caption(*game->getFontManager()->get("century_gothic"), "Music Volume: " + std::to_string(int(game->getSoundEngine()->getMusicVolume() * 100)) + "%");

	toggleGibsButton->setPosition(Vec2d((getWidth() - toggleGibsButton->getWidth())/2, getHeight()/10));
	toggleGibsCaption->setPosition(Vec2d((getWidth() - toggleGibsCaption->getWidth())/2, 2*getHeight()/10));

	voiceVolumeSlider->setPosition(Vec2d((getWidth() - voiceVolumeSlider->getWidth())/2, 3*getHeight()/10));
	voiceVolumeSlider->setValue(game->getPlayer()->getVoiceVolume());

	dukeQuipsVolumeCaption->setPosition(Vec2d((getWidth() - dukeQuipsVolumeCaption->getWidth())/2, 4*getHeight()/10));

	musicVolumeSlider->setPosition(Vec2d((getWidth() - musicVolumeSlider->getWidth())/2, 5*getHeight()/10));
	//musicVolumeSlider->setValue(game->getSoundEngine()->getMusicVolume());

    //musicVolumeCaption->setPosition(Vec2d((getWidth() - musicVolumeCaption->getWidth())/2, 6*getHeight()/10));
	
	difficultyCaption->setPosition(Vec2d((getWidth() - difficultyCaption->getWidth())/2, 9.5*getHeight()/10));
	difficultySlider->setPosition(Vec2d((getWidth() - difficultySlider->getWidth())/2, 8.5*getHeight()/10));
	difficultySlider->setValue(game->getHardness());

	addItem("Back", backButton);
	addItem("AllowGibs", toggleGibsButton);
	addItem("VoiceVolumeSlider", voiceVolumeSlider);
	addItem("MusicVolumeSlider", musicVolumeSlider);
	addItem("DifficultySlider", difficultySlider);

	addItem("DifficultyCaption", difficultyCaption);
	addItem("AllowGibsCaption", toggleGibsCaption);
	addItem("VoiceVolumeCaption", dukeQuipsVolumeCaption);
	//addItem("MusicVolumeCaption", musicVolumeCaption);

	setPosition((game->getDisplay()->getDimensions() - getDimensions()) / 2);


	gpEventSystem->addListener(EVENT_MENUCONTROL, this);

	mBackColor = Color(0.1f,0.1f,0.1f,1.0f);
}

void OptionsMenu::handleEvent( const Event& theEvent )
{
	switch(theEvent.getType())
	{
		case EVENT_MENUCONTROL:
		{

			MenuItem *control = ((EventMenuControl&)theEvent).getMenuItem();

			bool redraw = ((EventMenuControl&)theEvent).isRedrawOnly();

			if(control == mItems["Back"])
				gpEventSystem->fireEvent(EventMenuPop(this));
			else if(control == mItems["AllowGibs"])
			{
				game->toggleAllowGibs();
				//game->getPlayer()->attack(GLFW_MOUSE_BUTTON_2);
				((Caption*)mItems["AllowGibsCaption"])->setText(game->getAllowGibs() ? "Gibs: Lots of Gibs" : "Gibs: No Gibs");
				((Caption*)mItems["AllowGibsCaption"])->setPosition(Vec2d((getWidth() - ((Caption*)mItems["AllowGibsCaption"])->getWidth())/2, 2*getHeight()/10));

				if(game->getAllowGibs())
					game->getPlayer()->speak("sound/gibs1.ogg", true);
				else
					game->getPlayer()->speak("sound/lame1.ogg", true);
			}
			else if(control == mItems["VoiceVolumeSlider"])
			{
				//Slider *slider = ((Slider*)control);

				//if(!redraw)
					//game->getPlayer()->setVoiceVolume(slider->getValue());

				((Caption*)mItems["VoiceVolumeCaption"])->setText("Duke Quips Volume: " + std::to_string(int(game->getPlayer()->getVoiceVolume() * 100)) + "%");

				//if(!redraw && slider->wasJustReleased() && !game->getPlayer()->isSpeaking())
					//game->getPlayer()->speak(game->getPlayer()->getQuipKill(), true);
			}
			else if(control == mItems["MusicVolumeSlider"])
			{
				//Slider *slider = ((Slider*)control);

				//if(!redraw)
					//game->getSoundEngine()->setMusicVolume(slider->getValue());

				//((Caption*)mItems["MusicVolumeCaption"])->setText("Music Volume: " + std::to_string(int(game->getSoundEngine()->getMusicVolume() * 100)) + "%");
			}
			else if(control == mItems["DifficultySlider"])
			{
				Slider *slider = ((Slider*)control);

				if(!redraw)
				{
					float hardness = game->getHardness();

					float value = slider->getValue();

					if(hardness > 0.50f && value < 0.50f)
						game->getPlayer()->speak("sound/wuss1.ogg", true);
					else if(hardness < 1.50f && value > 1.50f)
						game->getPlayer()->speak("sound/yeah1.ogg", true);

					game->setHardness(value);
				}
			}
		}
		default:
			break;
	}
}
