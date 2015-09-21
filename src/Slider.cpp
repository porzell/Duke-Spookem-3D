#include "Slider.h"

#include "EventMouseDown.h"
#include "EventMouseMove.h"
#include "EventMouseUp.h"

#include "EventSystem.h"

#include "InputManager.h"

#include "GLincludes.h"

#include "Display.h"

#include "Game.h"

#include "Utils.h"

#include "ResourceManager.h"
#include "SoundEngine.h"

extern Game *game;

Slider::Slider(float length, Vec2d &valueScale, bool callOnMove, float defaultpos, Color backcolor, Color forecolor, Color selectcolor)
{
	mLength = length;
	mValueScale = valueScale;
	
	mTargetPosition = constrain(convertScale(defaultpos, mValueScale.X(), mValueScale.Y(), 0, mLength), 0, mLength);
	mSliderPosition = constrain(convertScale(defaultpos, mValueScale.X(), mValueScale.Y(), 0, mLength), 0, mLength);

	mCallOnMove = callOnMove;

	mForeColor = forecolor;
	mBackColor = backcolor;
	mSelectedColor = selectcolor;
	
	mDimensions.setX(length);
	
	mDimensions.setY(32);

	mEnabled = true;
	mHover = false;
	mPressed = false;

	mTouched = false;

	checkListeners();
}

Slider::~Slider()
{
}

/*void Slider::redraw()
{
	Color *color;

	if(mHover && !mPressed)
		color = &mSelectedColor;
	else
		color = &mBackColor;

	mpBuffer->fill(*color);

	//Draw outline.
	mpBuffer->drawRect(Vec2d(0,0), mpBuffer->getDimensions()-2, mForeColor, 2.0);

	//Draw Slider caption.
	mpBuffer->writeText(mText,(getDimensions() - mTextDimensions)/2, *mpFont, mForeColor);
}*/

void Slider::draw()
{
	if(mSliderPosition != mTargetPosition)
	{
		if(mTouched)
			trigger();
		else
			requestRedraw();
	}

	mSliderPosition += (mTargetPosition - mSliderPosition)/8.0f;

	Color *color;

	if(mHover && !mPressed)
		color = &mSelectedColor;
	else
		color = &mBackColor;

	glPushMatrix();

	glTranslatef(mPosition.X(), mPosition.Y(), 0);

	//***********
	//Draw Slider.
	//***********

	glLineWidth(5);

	glBegin(GL_LINES);
	
	mBackColor.setActiveColor();
	
	glVertex2f(0,16);
	glVertex2f(mLength,16);
	
	glEnd();
	
	glTranslatef( mSliderPosition, 0, 0);
	
	glBegin(GL_QUADS);
	
	color->setActiveColor();

	glVertex2f(0,0);
	glVertex2f(32, 0);
	glVertex2f(32, 32);
	glVertex2f(0, 32);

	glEnd();

	//************
	//Draw outline
	//************

	glBegin(GL_LINES);
	
	//Set line width to 2.0.
	glLineWidth(2.0f);

	mForeColor.setActiveColor();

	glVertex2f(0,0);
	glVertex2f(32, 0);
	
	glVertex2f(32, 0);
	glVertex2f(32, 32);
	
	glVertex2f(32, 32);
	glVertex2f(0, 32);
	
	glVertex2f(0, 32);
	glVertex2f(0, 0);

	glEnd();

	glPopMatrix();

	/*glPushMatrix();

	Vec2d center = (getDimensions() - mTextDimensions)/2;

	glTranslatef(center.X(), center.Y(), 0);
	mpFont->print(mText.c_str());

	glPopMatrix();*/

	//Vec2d center = (getDimensions() - mTextDimensions)/2;

	//mpFont->print(mText, center.X(), center.Y());

	//mpBuffer->draw(buffer,mPosition);
}

void Slider::checkListeners()
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

void Slider::handleEvent( const Event& theEvent )
{
	//Make sure the position corresponds.
	//mSliderPosition.setY(mPosition.Y());
	
	switch(theEvent.getType())
	{
		case EVENT_MOUSEMOVE:
		{		
			if(isWithinBounds(((EventMouseMove&)theEvent).getPosition()))
			{
				if(!mHover)
				{
					mHover = true;
					//redraw();
				}
			}
			else if(mHover)
			{
				mHover = false;
				//redraw();
			}
			
			if(mPressed)
			{
				mTargetPosition = constrain((((EventMouseMove&)theEvent).getPosition().X() - mPinPosition) - (mMenuPosition.X() + mPosition.X()), 0, mLength);
				
				if(mCallOnMove)
				{
					mJustReleased = false;
					trigger();
				}
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
						mTouched = true;

						game->getSoundEngine()->play2DSound(game->getResourceManager()->get("UI_hoversound"));

						//redraw();
						mPinPosition = ((EventMouseMove&)theEvent).getPosition().X() - (mSliderPosition + mPosition.X() + mMenuPosition.X());
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
				//if(isWithinBounds(event.getPosition()))
				//{
					if(mPressed)
					{
						mPressed = false;
						//redraw();

						game->getSoundEngine()->play2DSound(game->getResourceManager()->get("UI_hoversound"));

						//Let the menu know we have been pressed.
						mJustReleased = true;
						trigger();
						mJustReleased = false;
					}
				//}
				//else
					//mPressed = false;
			}

			break;
		}
	
		default:
			break;
	}
}

bool Slider::isWithinBounds(Vec2d &point)
{
	point.setY(game->getDisplay()->getDimensions().Y() - point.Y());

	Vec2d pos = mPosition + mMenuPosition;

	pos.addX(mSliderPosition);

	return (point.X() >= pos.X() &&
			point.Y() >= pos.Y() &&
			point.X() <= pos.X() + 32 &&
			point.Y() <= pos.Y() + 32
			);
}

float Slider::getValue()
{
	return convertScale(mSliderPosition, 0, mLength, mValueScale.X(), mValueScale.Y());
}

void Slider::setValue(float value)
{
	mTargetPosition = constrain(convertScale(value, mValueScale.X(), mValueScale.Y(), 0, mLength), 0, mLength);
}