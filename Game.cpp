/** @file Game.cpp */

#include "Game.h"
#include "MainLoop.h"
#include "Map.h"
#include "Savegame.h"
#include "QuestResourceList.h"
#include "LuaContext.h"
#include "Color.h"
#include "Surface.h"
#include "VideoManager.h"
#include <sstream>
#include <vector>

/**
 * \brief Creates a game.
 * \param main_loop The Solarus root object.
 * \param savegame The saved data of this game. Will be deleted in the
 * destructor unless someone is still using it (the refcount info is used).
 */
Game::Game(MainLoop& main_loop, Savegame* savegame):
	
	main_loop(main_loop),
	savegame(savegame),
	pause_key_available(true),
	paused(false),
	gameover_sequence(NULL),
	started(false),
	restarting(false),
	keys_effect(NULL),
	current_map(NULL),
	next_map(NULL),
	previous_map_surface(NULL),
	transition_style(Transition::IMMEDIATE),
	transition(NULL),
	crystal_state(false),
	dialog_box(*this)
{
	//Notify objects
	savegame->increment_refcount();
	savegame->set_game(this);
	
	//Initialize members
	commands = new GameCommands(*this);
	hero = new Hero(get_equipment());
	hero->increment_refcount();
	keys_effect = new KeysEffect();
	update_keys_effect();
	
	//Launch the new starting map.
	std::string starting_map_id = get_savegame().get_string(Savegame::KEY_STARTING_MAP);
	if(starting_map_id.empty())
	{
		//When no starting map is set, use the first one declared in the resource list file.
		const std::vector<std::string>& map_ids = 
			QuestResourceList::get_elements(QuestResourceList::RESOURCE_MAP);
		if(map_ids.empty())
		{
			std::cerr << "This quest has no map\n";
		}
		starting_map_id = map_ids[0];
	}
	set_current_map(starting_map_id,
		savegame->get_string(Savegame::KEY_STARTING_POINT), Transition::FADE);
		
}




















