#pragma once
#ifndef _SLIDER_H
#define _SLIDER_H

#include "Vec2d.h"

#include "Color.h"

//#include "EventListener.h"
#include "MenuItem.h"
#include <string>

class Slider : public MenuItem
{
	private:
		
		float mLength;
		Vec2d mValueScale;
		
		bool mTouched;

		float mSliderPosition;
		float mTargetPosition;
		
		Color mBackColor, mForeColor, mSelectedColor;

		Vec2d mDimensions, mTextDimensions;
		
		bool mCallOnMove;

		//Font *mpFont;

		bool mHover, mPressed;

		bool mJustReleased;

		//bool mIsMoving, mLetGo;
		//bool mIsMoving, mLetGo;
		
		float mPinPosition;

		//Set listeners to be active or inactive based on enabled state;
		void checkListeners();

	public:
		Slider(float length = 100.f, Vec2d &valueScale = Vec2d(0,1), bool callOnMove = false, float defaultpos = 0.0f, Color backcolor = Color(0.4f, 0.4f, 0.4f), Color forecolor = Color(1.0f, 1.0f, 1.0f), Color selectcolor = Color(0.75f,0.4f,0.4f));
		~Slider();

		//Draw Slider onto a buffer.
		//void draw(Texture &buffer);
		void draw();

		//Redraw Slider buffer after making a graphical change.
		//void redraw();

		//Get Slider dimensions.
		inline Vec2d getDimensions() { return mDimensions; };

		//Get Slider width.
		inline int getWidth() { return mDimensions.X(); };

		//Get Slider height.
		inline int getHeight() { return mDimensions.Y(); };

		//Check if point is within Slider bounds.
		bool isWithinBounds(Vec2d &point);
		
		//Get value of slider on provided scale.
		float getValue();

		//Set value of slider on provided scale.
		void setValue(float value);
		
		//CHeck if Slider is triggering because it was just pressed.
		inline bool wasJustReleased() { return mJustReleased; };

		//Handles incoming events from eventsystem.
		void handleEvent( const Event& theEvent );
};

#endif