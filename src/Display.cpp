#include "Display.h"

#include "Game.h"

//#include "Texture.h"

extern Game *game;

Display::Display()
{
	mCursorVisible = true;
	mFullscreen = false;
	mShouldTrapCursor = false;
	mpDisplay = NULL;

    fprintf(stderr, "GLFW Starting...\n");
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
		std::cerr << "ERROR: Display! " << error << std::endl;
	}
}

Display::~Display()
{
	cleanup();

	//Peter: This terminates GLFW (opposite of glfwInit(), basically.)
	//This should also get rid of any windows we opened so the cleanup function may no longer need to be called since we're using GLFW.
	glfwTerminate();
}

void Display::cleanup()
{
	if(mpDisplay != NULL)
	{
		glfwDestroyWindow(mpDisplay);
		mpDisplay = NULL;
	}
}

//Do things like poll events and keep window state updated.
void Display::think()
{
	glfwPollEvents();
}

//Set a function to be treated as a keyboard callback.
void Display::setKeyboardCallback(GLFWkeyfun callback)
{
	glfwSetKeyCallback(mpDisplay, callback);
}

//Set a function to be treated as a mouse movement callback.
void Display::setMouseMoveCallback(GLFWcursorposfun callback)
{
	glfwSetCursorPosCallback(mpDisplay, callback);
}

//Set a function to be treated as a mouse click callback.
void Display::setMouseClickCallback(GLFWmousebuttonfun callback)
{
	glfwSetMouseButtonCallback(mpDisplay, callback);
}

//Get width of display.
int Display::getWidth()
{
	int width, height;
	glfwGetWindowSize(mpDisplay, &width, &height);
	return width;
}

//Get height of display.
int Display::getHeight()
{
	int width, height;
	glfwGetWindowSize(mpDisplay, &width, &height);
	return height;
}

//Initialize the display.
bool Display::init(int width, int height, std::string title, bool fullscreen)
{
	cleanup();

	mFullscreen = fullscreen;

	//Peter: I fixed up some stuff with how the error handling works here so I don't cast a pointer to a boolean.

	bool worked = false;

	try
	{
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		mpDisplay = glfwCreateWindow(width, height, title.c_str(), mFullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

		//9/3/2014 - Fix for some display creation errors on a machine.
		glfwMakeContextCurrent(mpDisplay);

		if(mpDisplay == NULL)
		{
			throw "Could not create display";
		}
		else
		{
			worked = true;
			setAsBackbuffer();
			//TODO: Add window closing event
		}
	}
	catch(char* error)
	{
		std::cerr << "ERROR: Display! " << error << std::endl;
	}

	if(worked)
	{
		glewExperimental = GL_TRUE;

		GLenum err = glewInit();

		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

		}

		fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	}

	return worked;
}

void Display::setFullscreen(bool fullscreen)
{
	//TODO: Figure this stuff out; will probably have to remake window
}

//Initialize the display.
bool Display::init(Size &size, std::string title, bool fullscreen)
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
			setAsBackbuffer();
			//TODO: Add window closing event
		}
	}
	catch(char* error)
	{
		std::cerr << "ERROR: Display! " << error << std::endl;
	}

	if(worked)
	{
		glewExperimental = GL_TRUE;

		GLenum err = glewInit();

		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

		}

		fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	}

	return worked;
}

//Destroy our display object.
void Display::destroy()
{		
	if(mpDisplay != NULL)
	{
		glfwDestroyWindow(mpDisplay);
		mpDisplay = NULL;
	}
}

//Set game window title.
void Display::setTitle(const std::string &title)
{
	glfwSetWindowTitle(mpDisplay, title.c_str());
}

//Get cursor state.
bool Display::getCursorVisibility()
{
	return mCursorVisible;
}

//Toggle game cursor.
void Display::setCursorVisibility(bool showCursor)
{
	mCursorVisible = showCursor;

	if(mCursorVisible)
		glfwSetInputMode(mpDisplay, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(mpDisplay, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

//Set the display as the current backbuffer.
void Display::setAsBackbuffer()
{
	glfwMakeContextCurrent(mpDisplay);
}

//Get a Texture that gives access to the backbuffer.
/*Texture Display::getBackBuffer()
{
	return Texture(*this);
}*/

//Flip buffers.
void Display::flip()
{
	glfwSwapBuffers(mpDisplay);
}
