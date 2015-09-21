#pragma once
#ifndef _BUTTON_H
#define _BUTTON_H

#include "Color.h"

class Texture;
class Font;

//#include "EventListener.h"
#include "MenuItem.h"
#include <string>

class Button : public MenuItem
{
	private:
		Font *mpFont;

		Color mBackColor, mForeColor, mSelectedColor;

		Vec2d mDimensions, mTextDimensions;

		std::string mText;
		Texture *mpBuffer;
		//Font *mpFont;

		bool mHover, mPressed;

		//Set listeners to be active or inactive based on enabled state;
		void checkListeners();

	public:
		Button(Font &font, std::string text, Color backcolor = Color(0.4f, 0.4f, 0.4f, 0.5f), Color forecolor = Color(1.0f, 1.0f, 1.0f, 0.5f), Color selectcolor = Color(0.75f,0.4f,0.4f, 0.5f));
		/*Button(Font &font, Vec2d &size);
		Button(Font &font, Size &size);
		Button(Font &font, int width, int height);*/
		~Button();

		//Draw button onto a buffer.
		//void draw(Texture &buffer);
		void draw();

		//Redraw button buffer after making a graphical change.
		//void redraw();

		//Get button dimensions.
		inline Vec2d getDimensions() { return mDimensions; };

		//Get button width.
		inline int getWidth() { return mDimensions.X(); };

		//Get button height.
		inline int getHeight() { return mDimensions.Y(); };

		//Check if point is within Button bounds.
		bool isWithinBounds(Vec2d point);

		//Handles incoming events from eventsystem.
		void handleEvent( const Event& theEvent );
};

#endif