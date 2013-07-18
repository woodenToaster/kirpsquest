/** @file MainLoop.cpp
  *
  * Still need to implement get_debug_keys(), get_lua_context(), is_resetting()
  * set_resetting(), get_game(), set_game(), run(), notify_input(), update(), draw() */

#include "MainLoop.h"
#include "System.h"
#include "VideoManager.h"
#include "Color.h"
#include "Surface.h"
#include "LuaContext.h"
#include "QuestProperties.h"
#include "QuestResourceList.h"

/** @brief Missing debug_keys */

MainLoop::MainLoop(int argc, char** argv): root_surface(NULL), lua_context(NULL), exiting(false), game(NULL), next_game(NULL) 
{
	System::initialize(argc, argv);
	
	//Read the general properties of the quest
	QuestProperties quest_properties(*this);
	quest_properties.load();

	//Read the quest resource list from the file project_db.dat
	QuestResourceList::initialize();

	root_surface = new Surface(KQ_SCREEN_WIDTH, KQ_SCREEN_HEIGHT);
	root_surface->increment_refcount();

	lua_context = new LuaContext(*this);
	lua_context->initialize();

}

/** @brief Destructor missing lua_context, debug_keys */

MainLoop::~MainLoop()
{
	delete lua_context;
	root_surface->decrement_refcount();
	delete root_surface;
	QuestResourceList::quit();
	System::quit();
}

/** @brief Returns whether the user just closed the window 
 *  
 *  When this function returns true, stop everything immediately
 *  free memory and let the program quit.
 *
 *  @return true if the user wants to exit the program */

bool MainLoop::is_exiting()
{
	return exiting;
}

/** @brief Sets whether the user wants to quit the program. */

void MainLoop::set_exiting() 
{
  // Stop the program.
  exiting = true;
}


/**
* @brief Returns whether the program is being reset.
*/
bool MainLoop::is_resetting() 
{
  return game != NULL && next_game == NULL;
}

/**
* @brief Marks the current game as finished and sets the initial screen
* to be started at the next cycle.
*/
void MainLoop::set_resetting() 
{
  // Reset the program.
  set_game(NULL);
}

/**
* @brief Returns the current game if any.
* @return The game currently running or NULL.
*/
Game* MainLoop::get_game() 
{
  return game;
}

/**
* @brief Changes the game.
* @param game The new game to start, or NULL to start no game.
*/
void MainLoop::set_game(Game* game) 
{
  if (this->game != NULL) 
  {
    //this->game->stop();
  }

  this->next_game = game;
}
/** @brief Need LuaContext
 *  
 *  The main loop is executed here. The input events are forwarded
 *  to the current screen.  The current screen is redrawn when necessary. */

void MainLoop::run()
{
	InputEvent* event;
	uint32_t now;
	uint32_t next_frame_date = System::now();
	uint32_t frame_interval = 25;      //time interval between two drawings
	int delay;
	bool just_redrawn = false;  //to detect when the FPS number needs to be decreased

	//main loop
	while(!is_exiting())
	{
		//handle input events
	    event = InputEvent::get_event();
		if(event != NULL)
		{
			notify_input(*event);
			delete event;
		}

		//update the current screen
		update();

		/*//Add code to check if user wants to go to another game
		if(next_game != game)
		{
			if(game != NULL)
			{
				delete game;
			}
			game = next_game;
			if(game != NULL)
			{
				game->start();
			}
			else
			{
				//lua_context->exit();
				//lua_context->initialize();
				//Music::play(Music::none);
			}
		}
		else
		{*/
			now = System::now();
			delay = next_frame_date - now;
			//delay is the time remaining before next drawing

			if(delay <= 0)
			{
				//time to redraw
				if(just_redrawn && frame_interval <= 30)
				{
					//redraw less often
					frame_interval += 5;
				}
				next_frame_date = now + frame_interval;
				just_redrawn = true;
				draw();
			}
			else
			{
				just_redrawn = false;
				//sleep, if there's time
				System::sleep(1);
				if(delay >= 15)
				{
					//increase the FPS if there's a lot of time
					frame_interval--;
				}
			}
		//}
	}
	/*
	if(game != NULL)
	{
		game->stop();
		delete game;
		game = NULL;
	}*/
}

/** @brief Needs Game. 
 *  
 *  It handles the events common to all screens:
 *  closing the window, pressing F5 or a debug key.
 *  The notify_input() mehtod of the current screen
 *  is then called. */

void MainLoop::notify_input(InputEvent& event)
{
	if(event.is_window_closing())
	{
		exiting = true;
	}
	else if(event.is_keyboard_key_pressed())
	{
		//a key was pressed
		if(event.get_keyboard_key() == InputEvent::KEY_ESCAPE)
		{
			exiting = true;
		}
	}

	//send the event to Lua and to the current screen
	bool handled = lua_context->notify_input(event);
	/*if(!handled && game != NULL)
	{
		game->notify_input(event);
	}*/
}

/** @brief Needs Game */

void MainLoop::update()
{
	//'debug_keys->update();
	if(game != NULL)
	{
		//game->update();
	}
	lua_context->update();
	System::update();
}

/**
* @brief Needs Game. Redraws the current screen.
*
* This function is called repeatedly by the main loop.
*/
void MainLoop::draw() 
{
  root_surface->fill_with_color(Color::get_black());
  if (game != NULL) 
  {
   // game->draw(*root_surface);
  }
  lua_context->main_on_draw(*root_surface);
  VideoManager::get_instance()->draw(*root_surface);
}

/**
* \brief Returns the shared Lua context.
* \return The Lua context where all scripts are run.
*/
LuaContext& MainLoop::get_lua_context() 
{
  return *lua_context;
}