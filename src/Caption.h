#pragma once
#ifndef _CAPTION_H
#define _CAPTION_H

#include "Color.h"

#include "Font.h"

#include "MenuItem.h"
#include <string>

class Font;

class Caption : public MenuItem
{
	private:
		Color mColor;

		std::string mText;

		Font *mpFont;

		Vec2d mDimensions;

	public:
		Caption(Font &font, std::string text, Color forecolor = Color(1.0f, 1.0f, 1.0f));
		~Caption();

		//Draw caption.
		void draw();

		//Redraw caption buffer after making a graphical change.
		//void redraw();

		//Get Caption text.
		inline std::string getText() { return mText; }

		//Set caption text and redraw.
		inline void setText(std::string text) { mText = text; mDimensions = mpFont->getStringDimensionsVec2d(text); }

		//Get caption dimensions.
		inline Vec2d getDimensions() { return mDimensions; }

		//Get caption width.
		inline float getWidth() { return mDimensions.X(); }

		//Get caption height.
		inline float getHeight() { return mDimensions.Y(); }

		//Handles incoming events from eventsystem.
		void handleEvent( const Event& theEvent ) { }
};

#endif