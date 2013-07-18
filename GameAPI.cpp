/** @file GameAPI.cpp */

#include "LuaContext.h"
#include "MainLoop.h"
#include "Game.h"
#include "Savegame.h"
//#include "Equipment.h"
//#include "EquipmentItem.h"
#include "FileTools.h"
#include "lua.hpp"

const std::string LuaContext::game_module_name = "kq.game";

/**
* \brief Initializes the game features provided to Lua.
*/
void LuaContext::register_game_module() 
{
  static const luaL_Reg methods[] = {
      //{ "exists", game_api_exists },
      //{ "delete", game_api_delete },
      { "load", game_api_load },
      /*{ "save", game_api_save },
      { "start", game_api_start },
      { "is_started", game_api_is_started },
      { "is_suspended", game_api_is_suspended },
      { "is_paused", game_api_is_paused },
      { "set_paused", game_api_set_paused },
      { "get_map", game_api_get_map },
      { "get_value", game_api_get_value },
      { "set_value", game_api_set_value },
      { "get_starting_location", game_api_get_starting_location },
      { "set_starting_location", game_api_set_starting_location },
      { "get_life", game_api_get_life },
      { "set_life", game_api_set_life },
      { "add_life", game_api_add_life },
      { "remove_life", game_api_remove_life },
      { "get_max_life", game_api_get_max_life },
      { "set_max_life", game_api_set_max_life },
      { "add_max_life", game_api_add_max_life },
      { "get_money", game_api_get_money },
      { "set_money", game_api_set_money },
      { "add_money", game_api_add_money },
      { "remove_money", game_api_remove_money },
      { "get_max_money", game_api_get_max_money },
      { "set_max_money", game_api_set_max_money },
      { "get_magic", game_api_get_magic},
      { "set_magic", game_api_set_magic},
      { "add_magic", game_api_add_magic},
      { "remove_magic", game_api_remove_magic},
      { "get_max_magic", game_api_get_max_magic},
      { "set_max_magic", game_api_set_max_magic},
      { "has_ability", game_api_has_ability },
      { "get_ability", game_api_get_ability },
      { "set_ability", game_api_set_ability },
      { "get_item", game_api_get_item },
      { "has_item", game_api_has_item },
      { "get_item_assigned", game_api_get_item_assigned },
      { "set_item_assigned", game_api_set_item_assigned },
      { "is_command_pressed", game_api_is_command_pressed },
      { "get_commands_direction", game_api_get_commands_direction },
      { "get_command_effect", game_api_get_command_effect },
      { "get_command_keyboard_binding", game_api_get_command_keyboard_binding },
      { "set_command_keyboard_binding", game_api_set_command_keyboard_binding },
      { "get_command_joypad_binding", game_api_get_command_joypad_binding },
      { "set_command_joypad_binding", game_api_set_command_joypad_binding },
      { "capture_command_binding", game_api_capture_command_binding },*/
      { NULL, NULL }
  };
  static const luaL_Reg metamethods[] = {
      { "__gc", userdata_meta_gc },
      { "__newindex", userdata_meta_newindex_as_table },
      { "__index", userdata_meta_index_as_table },
      { NULL, NULL }
  };
  register_type(game_module_name, methods, metamethods);
}

/**
* \brief Pushes a game userdata onto the stack.
* \param l A Lua context.
* \param game A game.
*/
void LuaContext::push_game(lua_State* l, Savegame& game) 
{
  push_userdata(l, game);
}

/**
* \brief Implementation of kq.game.load().
* \param l The Lua context that is calling this function.
* \return Number of values to return to Lua.
*/
int LuaContext::game_api_load(lua_State* l)
{
	const std::string& file_name = luaL_checkstring(l, 1);

	if(FileTools::get_quest_write_dir().empty())
	{
		luaL_error(l, "Cannot load savegame:  no write directory was specified in quest.dat");
	}

	Savegame* savegame = new Savegame(get_lua_context(l).get_main_loop(), file_name);

	savegame->increment_refcount();
	//savegame->get_equipment().load_items();
	savegame->decrement_refcount();

	push_game(l, *savegame);
	return 1;
}