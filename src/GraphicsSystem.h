#pragma once
#ifndef _GRAPHICS_SYSTEM_H
#define _GRAPHICS_SYSTEM_H

#include <GLFW/glfw3.h>
#include <string>

#include "Trackable.h"

#include "Size.h"
#include "Vec2d.h"

//class GraphicsBuffer;

class GraphicsSystem : public Trackable
{
	public:
		GraphicsSystem();
		~GraphicsSystem();

		/*void drawGraphicsBuffer(GraphicsBuffer &buffer, const Vec2d &location);
		void drawGraphicsBufferScaled(GraphicsBuffer &buffer, const Vec2d &location);*/

		//Deals with unregistering the display event source and destroying the display.
		//(Called by init() and dtor.)
		void cleanup();

		//Get width of display.
		int getWidth();
	
		//Get height of display.
		int getHeight();
	
		//Get display dimensions.
		inline Vec2d getDimensions() { return Vec2d((float)getWidth(), (float)getHeight()); };

		//Initialize the display.
		bool init(int width, int height, std::string title = "Untitled Window", bool fullscreen = false);

		//Initialize the display.
		bool init(Size &size, std::string title = "Untitled Window", bool fullscreen = false);

		//Destroy our display object.
		void destroy();

		//Flip buffers;
		void flip();

		//Set game window title.
		void setTitle(const std::string &title);

		//Toggle fullscreen.
		void setFullscreen(bool fullscreen);

		//Get cursor state.
		bool getCursorVisibility();

		//Toggle game cursor.
		void setCursorVisibility(bool showCursor);

		//Set the display as the current backbuffer.
		void setAsBackbuffer();

		//Set whether premultiplied blendmode is on.
		void enablePreMultipliedAlpha(bool enable);

		//Check whether windows is supposed to be closed.
		bool shouldClose();

		//Do things like poll events and keep window state updated.
		void think();

		//Set a function to be treated as a keyboard callback.
		void setKeyboardCallback(GLFWkeyfun callback);

		//Set a function to be treated as a mouse movement callback.
		void setMouseMoveCallback(GLFWcursorposfun callback);

		//Set a function to be treated as a mouse click callback.
		void setMouseClickCallback(GLFWmousebuttonfun callback);

		//Get a GraphicsBuffer that gives access to the backbuffer.
		//GraphicsBuffer getBackBuffer();

		friend class Game;
		//friend class GraphicsBuffer;
		friend class Sprite;

	protected:
		GraphicsSystem(const GraphicsSystem&);
		GraphicsSystem& operator = (const GraphicsSystem&);

	private:
		GLFWwindow* mpDisplay;
		bool mCursorVisible, mFullscreen;
		static bool msFirstRun;
};

#endif
