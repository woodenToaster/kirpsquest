/** @file MainLoop.h */

#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "Common.h"
#include "Surface.h"
#include "ExportableToLua.h"
#include "InputEvent.h"
#include "Game.h"
#include "LuaContext.h"

class MainLoop
{
private:
	Surface* root_surface;		/**<surface where everything is drawn */
	LuaContext* lua_context;	/**<the Lua world where the scripts are run */
	bool exiting;				/**<Indicates that the program is about to stop */
	Game* game;					/**<The current game, if any, NULL otherwise. */
	Game* next_game;			/**<The game to start at next cycle (NULL means resetting the game). */

	void notify_input(InputEvent& event);
	void draw();
	void update();

public:
	MainLoop(int argc, char** argv);
	~MainLoop();

	void run();
	void set_exiting();
	bool is_exiting();
	void set_resetting();
	bool is_resetting();
	Game* get_game();
	void set_game(Game* game);

	LuaContext& get_lua_context();
};
#endif