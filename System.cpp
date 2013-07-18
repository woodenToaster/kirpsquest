/** @file System.cpp */

#include "System.h"
#include "SDL.h"
#include <iostream>
#include "VideoManager.h"
#include "InputEvent.h"
#include "FileTools.h"
#include "TextSurface.h"
#include "Sprite.h"
#include "Random.h"
#include "Sound.h"

uint32_t System::ticks = 0;

/** @brief Missing multiple initializations and destructors */

void System::initialize(int argc, char** argv)
{
	if((SDL_Init(SDL_INIT_VIDEO) == -1))
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError();
	}

	//files
	FileTools::initialize(argc, argv);

	//video
	VideoManager::initialize(argc, argv);
	Color::initialize();
	TextSurface::initialize();
	Sprite::initialize();

	//audio
	Sound::initialize(argc, argv);

	//input
	InputEvent::initialize();

	Random::initialize();
}


/** @brief Many objects need to be destroyed */

void System::quit() 
{
  Random::quit();
  InputEvent::quit();
  Sound::quit();
  //Sprite::quit();
  //TextSurface::quit();
  Color::quit();
  VideoManager::quit();
  //FileTools::quit();

  SDL_Quit();
}

/** @brief Add Sound::update() */

void System::update()
{
	ticks = SDL_GetTicks();
	//Sound::update();
}

/** @brief Returns the number of milliseconds elapsed since the beginning of the program */

uint32_t System::now()
{
	return ticks;
}

/** @brief Makes the program sleep 
 *  @param duration duration of the sleep in milliseconds */

void System::sleep(uint32_t duration)
{
	SDL_Delay(duration);
}