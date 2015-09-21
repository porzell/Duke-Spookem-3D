#pragma once
#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#include "Trackable.h"

#include "VectorMath.h"

struct GLFWwindow;

class InputManager : public Trackable
{
	private:

		static Vec2d mMousePos;

		//Key callback to be called by window when input is recieved.
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		//GLFWkeyfun key_callback;

		//Click callback to be called by window when mouse buttons are clicked.
		static void click_callback(GLFWwindow* window, int button, int action, int mods);
		//GLFWmousebuttonfun click_callback;

		//Mouse movement callback to be called by window when mouse is moved.
		static void move_callback(GLFWwindow* window, double x, double y);
		//GLFWcursorposfun move_callback;


	public:
		InputManager();
		~InputManager();

		//Return current mouse position as a Vec2d.
		inline Vec2d getMousePosition() { return mMousePos; };

		inline float getMouseX() { return mMousePos.X(); };
		inline float getMouseY() { return mMousePos.Y(); };
};

#endif