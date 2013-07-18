/** @file MainAPI.cpp */

#include "LuaContext.h"
#include "FileTools.h"
#include "MainLoop.h"
#include "Settings.h"
#include "lua.hpp"
#include <sstream>
#include <cmath>

const std::string LuaContext::main_module_name = "kq.main";

void LuaContext::register_main_module()
{
	static const luaL_Reg functions[] = 
	{
		{ "load_file", main_api_load_file }/*,
		{ "do_file", main_api_do_file },
		{ "reset", main_api_reset },
		{ "exit", main_api_exit },
		{ "is_debug_enabled", main_api_is_debug_enabled },
		{ "get_quest_write_dir", main_api_get_quest_write_dir },
		{ "set_quest_write_dir", main_api_set_quest_write_dir }*/,
		{ "load_settings", main_api_load_settings },
		{ "save_settings", main_api_save_settings },/*
		{ "get_distance", main_api_get_distance },
		{ "get_angle", main_api_get_angle },*/
		{ NULL, NULL }
	};
	register_functions(main_module_name, functions);
}

/**
* @brief Pushes the sol.main table onto the stack.
* @param l A Lua state.
*/
void LuaContext::push_main(lua_State* l)
{
	lua_getglobal(l, "kq");
	lua_getfield(l, -1, "main");
	lua_remove(l, -2);
}

int LuaContext::main_api_load_file(lua_State* l)
{
	const std::string& file_name = luaL_checkstring(l, 1);

	if(!load_file_if_exists(l, file_name))
	{
		lua_pushnil(l);
	}
	return 1;
}

int LuaContext::main_api_load_settings(lua_State* l)
{
	std::string file_name = luaL_optstring(l, 1, "settings.dat");
	bool success = Settings::load(file_name);
	lua_pushboolean(l, success);
	return 1;
}

/**
* \brief Implementation of sol.main.save_settings().
* \param l the Lua context that is calling this function
* \return number of values to return to Lua
*/
int LuaContext::main_api_save_settings(lua_State* l)
{
	std::string file_name = luaL_optstring(l, 1, "settings.dat");

	if(FileTools::get_quest_write_dir().empty())
	{
		luaL_error(l, "Cannot save settings: no write directory was specified in quest.dat");
	}

	bool success = Settings::save(file_name);

	lua_pushboolean(l, success);
	return 1;
}

void LuaContext::main_on_started()
{
	push_main(l);
	on_started();
	lua_pop(l, 1);
}

/**
* \brief Calls kq.main.on_finished() if it exists.
*
* This function is called when the program is reset or stopped.
*/
void LuaContext::main_on_finished()
{
	push_main(l);
	on_finished();
	remove_timers(-1);
	remove_menus(-1);
	lua_pop(l, 1);
}

/**
* @brief Calls kq.main.on_update() if it exists.
*
* This function is called at each cycle by the main loop.
*/
void LuaContext::main_on_update() 
{
	push_main(l);
	//on_update();  
	//menus_on_update(-1);
	lua_pop(l, 1);
}

/**
* @brief Calls kq.main.on_draw() if it exists.
* @param dst_surface The destination surface.
*/
void LuaContext::main_on_draw(Surface& dst_surface) 
{
	push_main(l);
	menus_on_draw(-1, dst_surface);
	on_draw(dst_surface);
	lua_pop(l, 1);
}

/**
* @brief Notifies Lua that an input event has just occurred.
*
* The appropriate callback in sol.main is triggered if it exists.
*
* @param event The input event to handle.
* @return \c true if the event was handled and should stop being propagated.
*/
bool LuaContext::main_on_input(InputEvent& event) 
{
  bool handled = false;
  push_main(l);
  handled = on_input(event);
  if (!handled) 
  {
    handled = menus_on_input(-1, event);
  }
  lua_pop(l, 1);
  return handled;
}

