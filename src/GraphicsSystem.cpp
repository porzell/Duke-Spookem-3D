#include "GraphicsSystem.h"

#include "Game.h"

//#include "GraphicsBuffer.h"

extern Game *game;

bool GraphicsSystem::msFirstRun = false;

GraphicsSystem::GraphicsSystem()
{
	mCursorVisible = true;
	mFullscreen = false;
	mpDisplay = NULL;

	//Peter: Need to init GLFW before trying to create a window.
	try
	{
		if(!glfwInit())
		{
			throw "GLFW could not be initialized!";
		}
	}
	catch(char* error)
	{
		std::cerr << "ERROR: GraphicsSystem! " << error << std::endl;
	}
}

GraphicsSystem::~GraphicsSystem()
{
	cleanup();

	//Peter: This terminates GLFW (opposite of glfwInit(), basically.)
	//This should also get rid of any windows we opened so the cleanup function may no longer need to be called since we're using GLFW.
	glfwTerminate();
}

void GraphicsSystem::cleanup()
{
	if(mpDisplay != NULL)
	{
		glfwDestroyWindow(mpDisplay);
		mpDisplay = NULL;
	}
}

//Check whether windows is supposed to be closed.
bool GraphicsSystem::shouldClose()
{
	return (glfwWindowShouldClose(mpDisplay) != 0);
}

//Do things like poll events and keep window state updated.
void GraphicsSystem::think()
{
	glfwPollEvents();
}

//Set a function to be treated as a keyboard callback.
void GraphicsSystem::setKeyboardCallback(GLFWkeyfun callback)
{
	glfwSetKeyCallback(mpDisplay, callback);
}

//Set a function to be treated as a mouse movement callback.
void GraphicsSystem::setMouseMoveCallback(GLFWcursorposfun callback)
{
	glfwSetCursorPosCallback(mpDisplay, callback);
}

//Set a function to be treated as a mouse click callback.
void GraphicsSystem::setMouseClickCallback(GLFWmousebuttonfun callback)
{
	glfwSetMouseButtonCallback(mpDisplay, callback);
}

//Get width of display.
int GraphicsSystem::getWidth()
{
	int width, height;
	glfwGetWindowSize(mpDisplay, &width, &height);
	return width;
}

//Get height of display.
int GraphicsSystem::getHeight()
{
	int width, height;
	glfwGetWindowSize(mpDisplay, &width, &height);
	return height;
}

//Initialize the display.
bool GraphicsSystem::init(int width, int height, std::string title, bool fullscreen)
{
	cleanup();

	//Peter: I fixed up some stuff with how the error handling works here so I don't cast a pointer to a boolean.

	bool worked = false;

	try
	{
		mpDisplay = glfwCreateWindow(width, height, title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

		if(mpDisplay == NULL)
		{
			throw "Could not create display";
		}
		else
		{
			worked = true;
			glfwMakeContextCurrent(mpDisplay);
			//TODO: Add window closing event
		}
	}
	catch(char* error)
	{
		std::cerr << "ERROR: GraphicsSystem! " << error << std::endl;
	}

	return worked;
}

void GraphicsSystem::setFullscreen(bool fullscreen)
{
	//TODO: Figure this stuff out; will probably have to remake window
}

//Initialize the display.
bool GraphicsSystem::init(Size &size, std::string title, bool fullscreen)
{
	cleanup();

	//Peter: I fixed up some stuff with how the error handling works here so I don't cast a pointer to a boolean.

	bool worked = false;

	try
	{
		mpDisplay = glfwCreateWindow(size.getWidth(), size.getHeight(), title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

		if(mpDisplay == NULL)
		{
			throw "Could not create display";
		}
		else
		{
			worked = true;
			glfwMakeContextCurrent(mpDisplay);
			//TODO: Add window closing event
		}
	}
	catch(char* error)
	{
		std::cerr << "ERROR: GraphicsSystem! " << error << std::endl;
	}
	return worked;
}

//Destroy our display object.
void GraphicsSystem::destroy()
{		
	if(mpDisplay != NULL)
	{
		glfwDestroyWindow(mpDisplay);
		mpDisplay = NULL;
	}
}

//Set game window title.
void GraphicsSystem::setTitle(const std::string &title)
{
	glfwSetWindowTitle(mpDisplay, title.c_str());
}

//Get cursor state.
bool GraphicsSystem::getCursorVisibility()
{
	return mCursorVisible;
}

//Toggle game cursor.
void GraphicsSystem::setCursorVisibility(bool showCursor)
{
	mCursorVisible = showCursor;

	if(mCursorVisible)
		glfwSetInputMode(mpDisplay, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(mpDisplay, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

//Set the display as the current backbuffer.
void GraphicsSystem::setAsBackbuffer()
{
	glfwMakeContextCurrent(mpDisplay);
}

//Get a GraphicsBuffer that gives access to the backbuffer.
/*GraphicsBuffer GraphicsSystem::getBackBuffer()
{
	return GraphicsBuffer(*this);
}*/

//Flip buffers.
void GraphicsSystem::flip()
{
	glfwSwapBuffers(mpDisplay);
}