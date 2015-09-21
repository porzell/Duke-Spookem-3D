#include "InputManager.h"

#include "Game.h"
#include "Display.h"
#include "Player.h"

#include "EventSystem.h"

#include "EventKeyUp.h"
#include "EventKeyDown.h"

#include "EventMouseUp.h"
#include "EventMouseMove.h"
#include "EventMouseDown.h"

#include "EventQuit.h"

#include "MenuStack.h"
#include "OptionsMenu.h"
#include "MainMenu.h"

extern Game *game;
extern EventSystem *gpEventSystem;

Vec2d InputManager::mMousePos = Vec2d(0,0);

void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::string actionString;

	switch(action)
	{
		case GLFW_RELEASE:
		{
			actionString = "Released: ";
			//game->getPlayer()->handleKey(false,key);
			gpEventSystem->fireEvent(EventKeyUp(key));
			break;
		}
		case GLFW_PRESS:
		{
			actionString = "Pressed: ";

			//game->getPlayer()->handleKey(true,key);

			gpEventSystem->fireEvent(EventKeyDown(key));

			if(key == GLFW_KEY_R)
			{
				game->toggleWireframeBoundingBoxMode();
			}
			else if(key == GLFW_KEY_ESCAPE)
				gpEventSystem->fireEvent(EventQuit());
				//game->getDisplay()->setShouldClose(true);
			break; 
		}
		case GLFW_REPEAT:
		{
			return;
			actionString = "Repeated: ";
			break;
		}
		default:
			break;
	}

	//game->shout("Keycode " + actionString + std::to_string(key) + '\n');
}

void InputManager::move_callback(GLFWwindow* window, double x, double y)
{
	//game->getPlayer()->handleMouse(Vec2d(float(x),float(y)) - mMousePos);

	Vec2d newPos = Vec2d(float(x),float(y));

	//Vec2d mousePos = Vec2d(x, game->getDisplay()->getHeight() - y);

	gpEventSystem->fireEvent(EventMouseMove(newPos, newPos - mMousePos));

	//mMousePos.set((float)x,(float)y);
	mMousePos = newPos;

	//game->shout("Mouse position: (" + std::to_string(int(x)) + "," + std::to_string(int(y)) + ")\n");
}

void InputManager::click_callback(GLFWwindow* window, int button, int action, int mods)
{
	std::string actionString;

	switch(action)
	{
		case GLFW_RELEASE:
		{
			actionString = " Released\n";
			gpEventSystem->fireEvent(EventMouseUp(button, mMousePos));
			break;
		}
		case GLFW_PRESS:
		{
			actionString = " Pressed\n";
			gpEventSystem->fireEvent(EventMouseDown(button, mMousePos));
			//game->getPlayer()->attack(button);
			break;
		}
		case GLFW_REPEAT:
		{
			actionString = " Repeated\n";
			break;
		}
		default:
			break;
	}

	//game->shout("Mouse Button " + std::to_string(button) + actionString);
}

InputManager::InputManager()
{
	game->getDisplay()->setKeyboardCallback(this->key_callback);
	game->getDisplay()->setMouseMoveCallback(this->move_callback);
	game->getDisplay()->setMouseClickCallback(this->click_callback);
}

InputManager::~InputManager()
{

}