#pragma once
#ifndef _MENU_H
#define _MENU_H

#include "EventListener.h"
#include <unordered_map>
#include "Vec2d.h"

#include <string>
 
class Size;
class MenuItem;
class Texture;

#include "Color.h"

class Menu : public EventListener
{
	protected:
		std::unordered_map<std::string, MenuItem*> mItems;

		Texture *mpTexture;

		Color mBackColor, mForeColor;
		
		Vec2d mPosition, mDimensions;

		bool mEnabled, mShouldDelete;

		bool mIsOpen;

		float opening;

	public:
		Menu(Size &size, Texture *texture = NULL, Color forecolor = Color(0.8f,0.8f,0.8f), Color backcolor = Color(0.3f,0.3f,0.3f,0.6f));
		Menu(Vec2d &size, Texture *texture = NULL, Color forecolor = Color(0.8f,0.8f,0.8f), Color backcolor = Color(0.3f,0.3f,0.3f,0.6f));
		Menu(float width, float height, Texture *texture = NULL, Color forecolor = Color(0.8f,0.8f,0.8f), Color backcolor = Color(0.3f,0.3f,0.3f,0.6f));

		virtual ~Menu();

		//Add a menu item to the menu.
		void addItem(std::string name, MenuItem *item);

		//Updates the menu GraphicsBuffer for if changes have been made.
		//virtual void redraw();

		//Get menu dimensions.
		inline Vec2d getDimensions() { return mDimensions; };

		//Get menu width.
		inline int getWidth() { return mDimensions.X(); };

		//Get menu height.
		inline int getHeight() { return mDimensions.Y(); };

		//Get control position.
		inline Vec2d getPosition() { return mPosition; };

		//Set control position.
		inline void setPosition(Vec2d position) { mPosition = position; updateControlPositions(); };

		//Draw the menu.
		virtual void draw();

		inline bool isEnabled() { return mEnabled; };
		inline bool shouldDelete() { return mShouldDelete; };

		void setEnabled(bool enabled);

		void updateControlPositions();

		//Handles incoming events from eventsystem.
		virtual void handleEvent( const Event& theEvent );
};

#endif
