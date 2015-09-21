#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "GLincludes.h"
#include <string>

#include "Trackable.h"

#include "Size.h"
#include "VectorMath.h"

#ifdef WINDOWS
#include <Windows.h>
#endif

//class Texture;

class Display : public Trackable
{
	public:
		Display();
		~Display();

		/*void drawTexture(Texture &buffer, const Vec2d &location);
		void drawTextureScaled(Texture &buffer, const Vec2d &location);*/

		//Deals with unregistering the display event source and destroying the display.
		//(Called by init() and dtor.)
		void cleanup();

		//Get width of display.
		int getWidth();
	
		//Get height of display.
		int getHeight();
	
		//Get display dimensions.
		inline Vec2d getDimensions() { return Vec2d((float)getWidth(), (float)getHeight()); }

		//Initialize the display.
		bool init(int width, int height, std::string title = "Untitled Display", bool fullscreen = false);

		//Initialize the display.
		bool init(Size &size, std::string title = "Untitled Display", bool fullscreen = false);

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

		//Set cursor position.
		inline void setCursorPosition(Vec2d pos){ glfwSetCursorPos(mpDisplay, pos.X(), pos.Y()); }

		//Get whether cursor is trapped.
		inline bool isCursorTrapped() { return mShouldTrapCursor; }

		//Set whether cursor should be trapped.
		inline void shouldTrapCursor( bool shouldTrap )
		{
			mShouldTrapCursor = shouldTrap;

			if(mShouldTrapCursor)
				glfwSetInputMode(mpDisplay, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(mpDisplay, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		}

		//Set the display as the current backbuffer.
		void setAsBackbuffer();

		//Check whether the display window is supposed to be closed.
		inline bool shouldClose() { return (glfwWindowShouldClose(mpDisplay) != 0); }

		//Set whether the display window is supposed to be closed.
		inline void setShouldClose(bool shouldClose) { glfwSetWindowShouldClose(mpDisplay, shouldClose); }

		//Do things like poll events and keep window state updated.
		void think();


		inline HWND getHwnd() { return GetForegroundWindow(); };
		inline HDC getHdc() { return GetDC( getHwnd() ); };

		//Set a function to be treated as a keyboard callback.
		void setKeyboardCallback(GLFWkeyfun callback);

		//Set a function to be treated as a mouse movement callback.
		void setMouseMoveCallback(GLFWcursorposfun callback);

		//Set a function to be treated as a mouse click callback.
		void setMouseClickCallback(GLFWmousebuttonfun callback);


		//Get a Texture that gives access to the backbuffer.
		//Texture getBackBuffer();

		friend class Game;

	protected:
		Display(const Display&);
		Display& operator = (const Display&);

	private:
		GLFWwindow* mpDisplay;
		bool mCursorVisible, mFullscreen, mShouldTrapCursor;
};

#endif
