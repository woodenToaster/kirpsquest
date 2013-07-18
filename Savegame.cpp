/** @file Savegame.cpp */

#include "Savegame.h"
#include "MainLoop.h"
#include "FileTools.h"
#include "InputEvent.h"
#include "LuaContext.h"
#include "lua.hpp"

/**
* \brief Creates a savegame with a specified file name, existing or not.
* \param main_loop The Solarus root object.
* \param file_name Name of the savegame file (can be a new file),
* relative to the quest write directory, with its extension.
*/
Savegame::Savegame(MainLoop& main_loop, const std::string& file_name): ExportableToLua(), file_name(file_name), 
																	   main_loop(main_loop), game(NULL)
{
	const std::string& quest_write_dir = FileTools::get_quest_write_dir();
	if(quest_write_dir.empty())
	{
		std::cerr << "The quest write directory for savegames was not set in quest.dat\n";
	}
	prefixed_file_name = quest_write_dir + "/" + file_name;

	if(!FileTools::data_file_exists(prefixed_file_name))
	{
		//This save does not exist yet.
		empty = true;
		set_initial_values();
	}
	else
	{
		//A save already exits.  Load it.
		empty = false;
		load();
	}
}

/**
* \brief Destructor.
*/
Savegame::~Savegame() 
{
}

/**
* \brief Returns the name of the file where the data is saved.
* \return the file name of this savegame
*/
const std::string& Savegame::get_file_name() 
{
  return file_name;
}

/**
* \brief Returns the Solarus main loop.
* \return The main loop.
*/
MainLoop& Savegame::get_main_loop() 
{
  return main_loop;
}

/**
* \brief Returns the Lua context where this savegame lives.
* \return The Lua context.
*/
LuaContext& Savegame::get_lua_context() 
{
  return main_loop.get_lua_context();
}

/**
* \brief Returns the player's equipment corresponding to this savegame.
* \return the equipment

Equipment& Savegame::get_equipment() 
{
  return equipment;
}
*/

/**
* \brief If this savegame is currently running in a game, return that game.
* \return A game or NULL.
*/
Game* Savegame::get_game() 
{
  return game;
}

/**
* \brief Sets the game that is running this savegame.
* \param game A game or NULL.
*/
void Savegame::set_game(Game* game) 
{
  this->game = game;
}

/**
* \brief Sets a string value saved.
* \param key Name of the value to set.
* \param value The string value to associate with this key.
*/
void Savegame::set_string(const std::string& key, const std::string& value) 
{
    if(!LuaContext::is_valid_lua_identifier(key))
	{
		std::cerr << "Savegame variable '" << key << "' is not a valid key\n";
	}

    saved_values[key].type = SavedValue::VALUE_STRING;
    saved_values[key].string_data = value;
}

/**
* \brief Sets an integer value saved.
* \param key Name of the value to set.
* \param value The integer value to associate with this key.
*/
void Savegame::set_integer(const std::string& key, int value)
{
	if(!LuaContext::is_valid_lua_identifier(key))
	{
		std::cerr << "Savegame variable '" << key << "' is not a valid key\n";
	}

	saved_values[key].type = SavedValue::VALUE_INTEGER;
	saved_values[key].int_data = value;
}

/**
* \brief Sets a boolean value saved.
* \param key Name of the value to set.
* \param value The boolean value to associate with this key.
*/
void Savegame::set_boolean(const std::string& key, bool value) 
{
  if(!LuaContext::is_valid_lua_identifier(key))
  {
	  std::cerr << "Savegame variable '" << key << "' is not a valid key\n";
  }

  saved_values[key].type = SavedValue::VALUE_BOOLEAN;
  saved_values[key].int_data = value;
}

/**
* \brief Returns the name identifying this type in Lua.
* \return The name identifying this type in Lua.
*/
const std::string& Savegame::get_lua_type_name() const 
{
  return LuaContext::game_module_name;
}

/**
* \brief Loads the initial values.
*/
void Savegame::set_initial_values() 
{
  /*
  // Set the initial controls.
  set_default_keyboard_controls();
  set_default_joypad_controls();
  */

  /*
  // Set the initial equipment.
  equipment.set_max_life(1);
  equipment.set_life(1);
  equipment.set_ability("tunic", 1); // Mandatory to have a valid hero sprite.
  */
}

/**
* \brief Reads the data from the savegame file.
*/
void Savegame::load()
{
	// Try to parse as Lua first.
	lua_State* l = luaL_newstate();
	size_t size;
	char* buffer;
	FileTools::data_file_open_buffer(prefixed_file_name, &buffer, &size);
	int result = luaL_loadbuffer(l, buffer, size, prefixed_file_name.c_str());
	FileTools::data_file_close_buffer(buffer);

	//Call Lua
	if(result == 0)
	{
		//The buffer was successfully loaded.

		//Make the Lua world aware of this savegame object.
		lua_pushlightuserdata(l, this);
		lua_setfield(l, LUA_REGISTRYINDEX, "savegame");

		//Set a special environment to catch every variable decleration.
		lua_newtable(l);
                                    // fun env
		lua_newtable(l);
                                    // fun env env_mt
		lua_pushcfunction(l, l_newindex);
                                    // fun env env_mt __newindex
		lua_setfield(l, -2, "__newindex");
                                    // fun env env_mt
		lua_setmetatable(l, -2);
                                    // fun env
		lua_setfenv(l, -2);
                                    // fun

		if(lua_pcall(l, 0, 0, 0) != 0)
		{
			std::cerr << "Failed to load savegame file '" << prefixed_file_name << "': " << lua_tostring(l, -1);
			lua_pop(l, 1);
		}
	}	
	lua_close(l);
}

/**
* \brief __newindex function of the environment of the savegame file.
*
* This special __newindex function catches declaration of global variables
* to store them into the savegame.
*
* \param l The Lua context that is calling this function.
* \return Number of values to return to Lua.
*/
int Savegame::l_newindex(lua_State* l)
{
	lua_getfield(l, LUA_REGISTRYINDEX, "savegame");
	Savegame* savegame = static_cast<Savegame*>(lua_touserdata(l, -1));
	lua_pop(l, 1);

	const std::string& key = luaL_checkstring(l, 2);

	switch(lua_type(l, 3))
	{
		case LUA_TBOOLEAN:
			savegame->set_boolean(key, lua_toboolean(l, 3));
			break;

		case LUA_TNUMBER:
			savegame->set_integer(key, int(lua_tointeger(l, 3)));
			break;

		case LUA_TSTRING:
			savegame->set_string(key, lua_tostring(l, 3));
			break;

		default:
			std::cerr << "Expected string, number or boolean, got" << luaL_typename(l, 3) << '\n';
	}
	return 0;
}
