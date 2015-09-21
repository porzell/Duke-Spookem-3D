#pragma once
#ifndef _MENU_ITEM_H

#include "EventListener.h"
#include "Texture.h"
#include "Vec2d.h"

class MenuItem : public EventListener
{
	protected:
		Vec2d mPosition;
		Vec2d mMenuPosition;

		bool mEnabled;

		//Set listeners to be active or inactive based on enabled state;
		virtual void checkListeners() { };

		//Sends a message to the menu saying that this control fired.
		void trigger();

		//Sends a redraw message to the menu.
		void requestRedraw();

	public:
		MenuItem();
		~MenuItem();

		//Set whether menu item is enabled.
		inline void setEnabled(bool enabled) { mEnabled = enabled; checkListeners(); };

		//See whether menu item is enabled.
		inline bool isEnabled() { return mEnabled; };

		inline Vec2d getPosition() { return mPosition; };
		inline void setPosition(Vec2d position) { mPosition = position; };

		//Get Menu position.
		inline Vec2d getMenuPosition() { return mMenuPosition; };

		//Set Menu position.
		inline void setMenuPosition(Vec2d position) { mMenuPosition = position; };

		virtual void draw();

		virtual void handleEvent( const Event& theEvent );
};

#endif
