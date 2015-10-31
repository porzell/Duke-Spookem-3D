/*
Author: Peter Orzell, Hugues Ross
Class: EGP-310-01
Assignment: Final Project
Certification of Authenticity:
We certify that this assignment is entirely our own work.
*/

#include <iostream>
#include <string>
#include <ctime>

#include "Trackable.h"

#include "MemoryTracker.h"

//#include "Timer.h"
//#include "PerformanceTracker.h"

#include "Game.h"

#include "Size.h"

#ifdef WINDOWS
#include <Windows.h>
#endif

//#include "Overlay.h"

#include "EventSystem.h"

//Define the game pointer which is used externally by a bunch of essential classes.
Game *game;

extern EventSystem* gpEventSystem;

/*#ifdef WINDOWS
int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
{
	srand(time(NULL));

	gpEventSystem = New EventSystem();

	//Allocate a new game object.
	game = New Game();

	//Initialize the game.
	game->init();

	//Start up the gameloop.
	game->doLoop();

	//When done, delete the game.
	delete game;

	delete gpEventSystem;

	//delete gpEventSystem;

	gMemoryTracker.reportAllocations(std::cout);

	//system("pause");

	return 0;
}
#endif*/


int main(int argc, char** argv)
{
	srand(time(NULL));

	gpEventSystem = New EventSystem();

	//Allocate a new game object.
	game = New Game();

	//Initialize the game.
	game->init();

	//Start up the gameloop.
	game->doLoop();

	//When done, delete the game.
	delete game;

	delete gpEventSystem;

	//delete gpEventSystem;

	gMemoryTracker.reportAllocations(std::cout);

	//system("pause");

	return 0;
}
