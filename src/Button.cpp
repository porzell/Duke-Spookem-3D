#include "Button.h"

#include "Texture.h"

#include "EventMouseDown.h"
#include "EventMouseMove.h"
#include "EventMouseUp.h"

#include "EventSystem.h"

#include "InputManager.h"

#include "GLincludes.h"

#include "Font.h"

#include "Display.h"

#include "Game.h"

#include "SoundEngine.h"

#include "ResourceManager.h"

extern Game *game;

extern EventSystem *gpEventSystem;

Button::Button(Font &font, std::string text, Color backcolor, Color forecolor, Color selectcolor)
{
	mpFont = &font;

	mText = text;

	mTextDimensions = mpFont->getStringDimensionsVec2d(text);

	//The 1.2 provides padding.
	mDimensions = mTextDimensions * Vec2d(1.2, 2.0);

	//For testing since font is screwed up.
	//mDimensions.setX(25);

	mForeColor = forecolor;
	mBackColor = backcolor;
	mSelectedColor = selectcolor;

	//redraw();

	mEnabled = true;
	mHover = false;
	mPressed = false;

	checkListeners();
}

Button::~Button()
{
}

/*void Button::redraw()
{
	Color *color;

	if(mHover && !mPressed)
		color = &mSelectedColor;
	else
		color = &mBackColor;

	mpBuffer->fill(*color);

	//Draw outline.
	mpBuffer->drawRect(Vec2d(0,0), mpBuffer->getDimensions()-2, mForeColor, 2.0);

	//Draw button caption.
	mpBuffer->writeText(mText,(getDimensions() - mTextDimensions)/2, *mpFont, mForeColor);
}*/

void Button::draw()
{
	Color *color;

	if(mHover && !mPressed)
		color = &mSelectedColor;
	else
		color = &mBackColor;

	/*mpBuffer->fill(*color);

	//Draw outline.
	mpBuffer->drawRect(Vec2d(0,0), mpBuffer->getDimensions()-2, mForeColor, 2.0);

	//Draw button caption.
	mpBuffer->writeText(mText,(getDimensions() - mTextDimensions)/2, *mpFont, mForeColor);*/

	glPushMatrix();

	glTranslatef(mPosition.X(), mPosition.Y(), 0);

	//***********
	//Draw button.
	//***********

	glBegin(GL_QUADS);
	
	color->setActiveColor();

	glVertex2f(0,0);
	glVertex2f(mDimensions.X(), 0);
	glVertex2f(mDimensions.X(), mDimensions.Y());
	glVertex2f(0, mDimensions.Y());

	glEnd();

	//************
	//Draw outline
	//************

	glBegin(GL_LINES);
	
	//Set line width to 2.0.
	glLineWidth(2.0f);

	mForeColor.setActiveColor();

	glVertex2f(0,0);
	glVertex2f(mDimensions.X(), 0);

	glVertex2f(mDimensions.X(), 0);
	glVertex2f(mDimensions.X(), mDimensions.Y());

	glVertex2f(mDimensions.X(), mDimensions.Y());
	glVertex2f(0, mDimensions.Y());

	glVertex2f(0, mDimensions.Y());
	glVertex2f(0,0);

	glEnd();

	glPopMatrix();

	/*glPushMatrix();

	Vec2d center = (getDimensions() - mTextDimensions)/2;

	glTranslatef(center.X(), center.Y(), 0);
	mpFont->print(mText.c_str());

	glPopMatrix();*/

	Vec2d center = mPosition + (getDimensions() - mTextDimensions)/2;

	mpFont->print(mText, center.X(), center.Y());

	//mpBuffer->draw(buffer,mPosition);
}

void Button::checkListeners()
{
	if(mEnabled)
	{
		gpEventSystem->addListener(EVENT_MOUSEDOWN, this);
		gpEventSystem->addListener(EVENT_MOUSEMOVE, this);
		gpEventSystem->addListener(EVENT_MOUSEUP, this);
	}
	else
		gpEventSystem->removeListenerFromAllEvents(this);
}

void Button::handleEvent( const Event& theEvent )
{
	switch(theEvent.getType())
	{
		case EVENT_MOUSEMOVE:
		{
			if(isWithinBounds(((EventMouseMove&)theEvent).getPosition()))
			{
				if(!mHover)
				{
					mHover = true;
					game->getSoundEngine()->play2DSound(game->getResourceManager()->get("UI_hoversound"));
					//redraw();
				}
			}
			else if(mHover)
			{
				mHover = false;
				//redraw();
			}
			break;
		}

		case EVENT_MOUSEDOWN:
		{
			EventMouseDown event = ((EventMouseDown&)theEvent);
			if(event.getButton() == GLFW_MOUSE_BUTTON_1)
			{
				if(isWithinBounds(event.getPosition()))
				{
					if(!mPressed)
					{
						mPressed = true;
						//redraw();
					}
				}
			}
			break;
		}

		case EVENT_MOUSEUP:
		{
			EventMouseUp event = ((EventMouseUp&)theEvent);
			if(event.getButton() == GLFW_MOUSE_BUTTON_1)
			{
				if(isWithinBounds(event.getPosition()))
				{
					if(mPressed)
					{
						mPressed = false;
						//redraw();

						game->getSoundEngine()->play2DSound(game->getResourceManager()->get("UI_selectsound"));

						//Let the menu know we have been pressed.
						trigger();
					}
				}
				else
					mPressed = false;
			}

			break;
		}
	
		default:
			break;
	}
}

bool Button::isWithinBounds(Vec2d point)
{
	point.setY(game->getDisplay()->getDimensions().Y() - point.Y());

	/*point *= -1;

	point += 1.0f;*/

	Vec2d pos = mPosition + mMenuPosition;
	return (point.X() >= pos.X() &&
			point.Y() >= pos.Y() &&
			point.X() <= pos.X() + getWidth() &&
			point.Y() <= pos.Y() + getHeight()
			);
}