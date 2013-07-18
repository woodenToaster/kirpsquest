/* @file LuaContext.cpp */

#include "LuaContext.h"
#include "FileTools.h"
#include <string>
#include <sstream>
#include <cassert>

std::map<lua_State*, LuaContext*> LuaContext::lua_contexts;

LuaContext::LuaContext(MainLoop& main_loop): l(NULL), main_loop(main_loop) 
{
}

LuaContext::~LuaContext()
{
	this->exit();
}

/**
* @brief Returns the LuaContext object that encapsulates a Lua state.
* @param l A Lua state.
* @return The LuaContext object encapsulating this Lua state.
*/
LuaContext& LuaContext::get_lua_context(lua_State* l) 
{
  std::map<lua_State*, LuaContext*>::iterator it = lua_contexts.find(l);
  return *it->second;
}

/**
* \brief Returns the Solarus main loop object.
* \return The main loop manager.
*/
MainLoop& LuaContext::get_main_loop() 
{
  return main_loop;
}

/** @brief Initializes Lua */
void LuaContext::initialize()
{
	l = luaL_newstate();
	//lua_atpanic(l, l_panic);
	luaL_openlibs(l);

	//Associate this LuaContext object with the lua_State pointer
	lua_contexts[l] = this;

	//Table to keep track of all userdata
	lua_newtable(l);
	lua_newtable(l);
	lua_pushstring(l, "v");
	lua_setfield(l, -2, "__mode");
	lua_setmetatable(l, -2);
	lua_setfield(l, LUA_REGISTRYINDEX, "kq.all_userdata");

	//Allow userdata to be indexable 
	lua_newtable(l);
	lua_setfield(l, LUA_REGISTRYINDEX, "kq.userdata_tables");

	//Create the kq table that will contain the whole Kirp's Quest API
	lua_newtable(l);
	lua_setglobal(l, "kq");

	//Register the C++ functions and types accessible by Lua
	register_modules();

	//Make require() able to load Lua files even from the data.kq archive
	lua_getglobal(l, "kq");
	lua_pushcfunction(l, l_loader);
	lua_setfield(l, -2, "loader");
	luaL_dostring(l, "table.insert(package.loaders, 2, kq.loader)");
	lua_pushnil(l);
	lua_setfield(l, -2, "loader");
	lua_pop(l, 1);

	//Execute the main file
	do_file_if_exists(l, "main");
	main_on_started();	
}

/**
* \brief Cleans Lua.
*/
void LuaContext::exit()
{
	if(l != NULL)
	{
		//Call kq.main.on_finished() if it exists
		main_on_finished();

		//Destroy unfinished objects
		destroy_menus();
		destroy_timers();
		destroy_drawables();

		//Finalize lua
		lua_close(l);
		lua_contexts.erase(l);
		l = NULL;
	}
}

void LuaContext::update()
{
	update_drawables();
	update_menus();
	update_timers();

	//main_on_update();
}

/**
* @brief Notifies Lua that an input event has just occurred.
*
* The appropriate callback in sol.main is notified.
*
* @param event The input event to handle.
* @return \c true if the event was handled and should stop being propagated.
*/
bool LuaContext::notify_input(InputEvent& event) 
{
  // Call the appropriate callback in sol.main (if it exists).
  return main_on_input(event);
}

/**
* \brief Returns whether the specified name is a valid Lua identifier.
* \param name The name to check.
* \return true if the name only contains alphanumeric characters or '_' and
* does not start with a digit.
*/
bool LuaContext::is_valid_lua_identifier(const std::string& name)
{
	if (name.empty() || (name[0] >= '0' && name[0] <= '9')) 
	{
		return false;
    }

	bool valid = true;
	for (std::string::const_iterator it = name.begin(); it != name.end() && valid; it++) 
	{
		char character = *it;
		valid = (character == '_' ||
			(character >= 'a' && character <= 'z') ||
			(character >= 'A' && character <= 'Z') ||
			(character >= '0' && character <= '9'));
    }
    return valid;
}

/**
* @brief Defines some C++ functions into a Lua table.
* @param module_name name of the table that will contain the functions
* (e.g. "kq.main")
* @param functions list of functions to define in the table
* (must end with {NULLL, NULL})
*/
void LuaContext::register_functions(const std::string& module_name, const luaL_Reg* functions)
{
	//create a table and fill it with the functions
	luaL_register(l, module_name.c_str(), functions);
	lua_pop(l, 1);
}

/**
* @brief Defines some C++ functions into a new Lua userdata type.
* @param module_name name of the table that will contain the functions
* (e.g. "sol.movement") - this string will also identify the type
* @param functions list of functions to define on the type
* (must end with {NULLL, NULL})
* @param metamethods metamethods to define on the type (can be NULL)
*/
void LuaContext::register_type(const std::string& module_name, const luaL_Reg* methods, const luaL_Reg* metamethods) 
{
  // create a table and fill it with the methods
  luaL_register(l, module_name.c_str(), methods);
                                  // module
  // create the metatable for the type, add it to the Lua registry
  luaL_newmetatable(l, module_name.c_str());
                                  // module mt
  if (metamethods != NULL) 
  {
    // fill the metatable
    luaL_register(l, NULL, metamethods);
                                  // module mt
  }

  // make metatable.__index = module
  // (or metatable.usual_index = module if __index is already defined)
  lua_getfield(l, -1, "__index");
                                  // module mt __index/nil
  lua_pushvalue(l, -3);
                                  // module mt __index/nil module
  if (lua_isnil(l, -2)) 
  {
                                  // module mt nil module
    lua_setfield(l, -3, "__index");
                                  // module mt nil
  }
  else 
  {
                                  // module mt __index module
    lua_setfield(l, -3, "usual_index");
                                  // module mt __index
  }
  lua_pop(l, 3);
                                  // --
}


void LuaContext::register_modules()
{
	// modules available to all scripts
	register_main_module();
	//register_game_module();
	//register_map_module();
	//register_entity_module();
	register_audio_module();
	register_timer_module();
	register_surface_module();
	register_text_surface_module();
	//register_sprite_module();
	//register_movement_module();
	//register_item_module();
	//register_input_module();
	//register_video_module();*/
	register_menu_module();
	//register_language_module();
}

/**
* @brief Pushes a Lua value from its reference.
* @param l A Lua state.
* @param ref A Lua reference.
*/
void LuaContext::push_ref(lua_State* l, int ref) 
{
  lua_rawgeti(l, LUA_REGISTRYINDEX, ref);
}

/**
* @brief Pushes a string.
* @param l A Lua state.
* @param text A string value.
*/
void LuaContext::push_string(lua_State* l, const std::string& text) 
{
  lua_pushstring(l, text.c_str());
}

void LuaContext::push_userdata(lua_State* l, ExportableToLua& userdata)
{
	//See if this userdata already exists
	lua_getfield(l, LUA_REGISTRYINDEX, "kq.all_userdata");

	lua_pushlightuserdata(l, &userdata);

	lua_gettable(l, -2);

	if(!lua_isnil(l, -1))
	{
		lua_remove(l, -2);
	}
	else
	{
		//Create new userdata
		lua_pop(l, 1);
		lua_pushlightuserdata(l, &userdata);
		userdata.increment_refcount();
		ExportableToLua** block_address = static_cast<ExportableToLua**>(lua_newuserdata(l, sizeof(ExportableToLua*)));
		*block_address = &userdata;
		luaL_getmetatable(l, userdata.get_lua_type_name().c_str());
		
		
		lua_getfield(l, -1, "__gc");
		
		lua_pop(l, 1);
		lua_setmetatable(l, -2);

		//keep track of the new userdata
		lua_pushvalue(l, -1);
		lua_insert(l, -4);
		lua_settable(l, -3);
		lua_pop(l, 1);
	}
}

/**
* @brief Checks that the value at the given index is userdata of the
* specified type and returns it.
* @param l a Lua state
* @param index an index in the Lua stack
* @param module_name name identifying the userdata type
* @return the userdata at this index
*/
ExportableToLua& LuaContext::check_userdata(lua_State* l, int index, const std::string& module_name) 
{
  index = get_positive_index(l, index);

  ExportableToLua** userdata = static_cast<ExportableToLua**>(luaL_checkudata(l, index, module_name.c_str()));
  return **userdata;
}

/**
* @brief Checks that the value at the given index is a color and returns it.
* @param l a Lua state
* @param index an index in the Lua stack
* @return the color at this index
*/
Color LuaContext::check_color(lua_State* l, int index) 
{
  index = get_positive_index(l, index);

  luaL_checktype(l, index, LUA_TTABLE);
  lua_rawgeti(l, index, 1);
  lua_rawgeti(l, index, 2);
  lua_rawgeti(l, index, 3);
  Color color(luaL_checkint(l, -3),
    luaL_checkint(l, -2),
    luaL_checkint(l, -1));
  lua_pop(l, 3);

  return color;
}

/**
* @brief Finalizer of a userdata type.
* @param l a Lua state
* @return number of values to return to Lua
*/
int LuaContext::userdata_meta_gc(lua_State* l) 
{
  ExportableToLua* userdata = *(static_cast<ExportableToLua**>(lua_touserdata(l, 1)));

  // Note that the userdata disappears from Lua but it may come back later!
  // So we need to keep its table if the refcount is not zero.
  // The full userdata is destroyed, but if the refcount is zero, the light
  // userdata and its table persists.

  // We don't need to remove the entry from kq.all_userdata
  // because it is already done: that table is weak on its values and the
  // value was the full userdata.

  userdata->decrement_refcount();
  if (userdata->get_refcount() == 0) 
  {
    // Remove the userdata from the list of userdata tables.
    // Otherwise, if the same pointer gets reallocated, the userdata will get
    // its table from this deleted one!
                                    // udata
    lua_getfield(l, LUA_REGISTRYINDEX, "kq.userdata_tables");
                                    // udata all_udata
    lua_pushlightuserdata(l, userdata);
                                    // udata all_udata lightudata
    lua_pushnil(l);
                                    // udata all_udata lightudata nil
    lua_settable(l, -3);
                                    // udata all_udata
    lua_pop(l, 1);
                                    // udata
    delete userdata;
  }

  return 0;
}

/**
* \brief Implementation of __newindex that allows userdata to be like tables.
*
* Lua code can make "object[key] = value" if object is a userdata with this
* __newindex metamethod.
*
* This metamethod must be used with its corresponding __index
* metamethod (see userdata_meta_index_as_table).
*
* \param l The Lua context that is calling this function.
* \return Number of values to return to Lua.
*/
int LuaContext::userdata_meta_newindex_as_table(lua_State* l)
{
  luaL_checktype(l, 1, LUA_TUSERDATA);
  luaL_checkany(l, 2);
  luaL_checkany(l, 3);

  ExportableToLua* userdata =
      *(static_cast<ExportableToLua**>(lua_touserdata(l, 1)));

  /* The user wants to make udata[key] = value but udata is a userdata.
   * So what we make instead is udata_tables[udata][key] = value.
   * This redirection is totally transparent from the Lua side.
   */

  lua_getfield(l, LUA_REGISTRYINDEX, "kq.userdata_tables");
                                  // ... udata_tables
  lua_pushlightuserdata(l, userdata);
                                  // ... udata_tables lightudata
  lua_gettable(l, -2);
                                  // ... udata_tables udata_table/nil
  if (lua_isnil(l, -1)) 
  {
    // Create the userdata table if it does not exist yet.
                                  // ... udata_tables nil
    lua_pop(l, 1);
                                  // ... udata_tables
    lua_newtable(l);
                                  // ... udata_tables udata_table
    lua_pushlightuserdata(l, userdata);
                                  // ... udata_tables udata_table lightudata
    lua_pushvalue(l, -2);
                                  // ... udata_tables udata_table lightudata udata_table
    lua_settable(l, -4);
                                  // ... udata_tables udata_table
  }
  lua_pushvalue(l, 2);
                                  // ... udata_tables udata_table key
  lua_pushvalue(l, 3);
                                  // ... udata_tables udata_table key value
  lua_settable(l, -3);
                                  // ... udata_tables udata_table
  return 0;
}

/**
* \brief Implementation of __index that allows userdata to be like tables.
*
* Lua code can get "object[key]" for an arbitrary key previously set.
*
* This metamethod must be used with its corresponding __newindex
* metamethod (see userdata_meta_newindex_as_table).
*
* \brief Implementation of __index for the type game.
* \param l The Lua context that is calling this function.
* \return Number of values to return to Lua.
*/
int LuaContext::userdata_meta_index_as_table(lua_State* l) 
{
   /* The user wants to make udata[key] but udata is a userdata.
	* So what we retrieve instead is udata_tables[udata][key].
	* This redirection is totally transparent from the Lua side.
	* If udata_tables[udata][key] does not exist, we fall back
	* to the usual __index for userdata, i.e. we look for a method
	* in its type.
	*/

  luaL_checktype(l, 1, LUA_TUSERDATA);
  luaL_checkany(l, 2);

  ExportableToLua* userdata =
      *(static_cast<ExportableToLua**>(lua_touserdata(l, 1)));

  bool found = false;
  lua_getfield(l, LUA_REGISTRYINDEX, "kq.userdata_tables");
                                  // ... udata_tables
  lua_pushlightuserdata(l, userdata);
                                  // ... udata_tables lightudata
  lua_gettable(l, -2);
                                  // ... udata_tables udata_table/nil
  if (!lua_isnil(l, -1)) 
  {
    lua_pushvalue(l, 2);
                                  // ... udata_tables udata_table key
    lua_gettable(l, -2);
                                  // ... udata_tables udata_table value
    found = !lua_isnil(l, -1);
  }

  // Nothing in the userdata's table: do the usual __index instead
  // (look in the userdata's type).
  if (!found) 
  {
    lua_getmetatable(l, 1);
                                  // ... meta
    lua_getfield(l, -1, "usual_index");
                                  // ... meta module
    lua_pushvalue(l, 2);
                                  // ... meta module key
    lua_gettable(l, -2);
                                  // ... meta module value
  }

  return 1;
}

const std::string LuaContext::check_string_field(lua_State* l, int table_index, const std::string& key)
{
	lua_getfield(l, table_index, key.c_str());
	//TODO error checking
	const std::string& value = lua_tostring(l, -1);
	lua_pop(l, 1);
	return value;
}

/**
* @brief Like check_string_field() but with a default value.
*
* This function acts like lua_getfield() followed by luaL_optstring().
*
* @param l A Lua state.
* @param table_index Index of a table in the stack.
* @param key Key of the field to get in that table.
* @param default_value The default value to return if the field is \c nil.
* @return The wanted field as a string.
*/
const std::string LuaContext::opt_string_field(lua_State* l, int table_index, const std::string& key,
		const std::string& default_value)
{
	lua_getfield(l, table_index, key.c_str());
	std::string value;
	if(lua_isnil(l, -1))
	{
		value = default_value;
	}
	else
	{
		if(!lua_isstring(l, -1))
		{
			std::cerr << "Error: string expected.\n";
		}
		value = lua_tostring(l, -1);
	}
	lua_pop(l, 1);
	return value;
}
/**
* @brief Like check_int_field() but with a default value.
*
* This function acts like lua_getfield() followed by luaL_optint().
*
* @param l A Lua state.
* @param table_index Index of a table in the stack.
* @param key Key of the field to get in that table.
* @param default_value The default value to return if the field is \c nil.
* @return The wanted field as an integer.
*/
int LuaContext::opt_int_field(lua_State* l, int table_index, const std::string& key, int default_value) 
{
  lua_getfield(l, table_index, key.c_str());
  int value = default_value;
  if (!lua_isnil(l, -1)) 
  {
    /*if (!lua_isnumber(l, -1)) {
      luaL_argerror(l, table_index, (StringConcat() <<
          "Bad field '" << key << "' (integer expected, got " <<
          luaL_typename(l, -1) << ")").c_str()
      );*/
    value = int(lua_tointeger(l, -1));
  }

  lua_pop(l, 1);
  return value;
}

/**
* @brief Like check_boolean_field() but with a default value.
*
* This function acts like lua_getfield() followed by lua_toboolean().
*
* @param l A Lua state.
* @param table_index Index of a table in the stack.
* @param key Key of the field to get in that table.
* @param default_value The default value to return if the field is \c nil.
* @return The wanted field as a string.
*/
bool LuaContext::opt_boolean_field(lua_State* l, int table_index, const std::string& key, bool default_value)
{
	lua_getfield(l, table_index, key.c_str());
	bool value = default_value;
	if (!lua_isnil(l, -1)) 
	{
		/*
		if (lua_type(l, -1) != LUA_TBOOLEAN) 
		{
			luaL_argerror(l, table_index, (StringConcat() <<
			"Bad field '" << key << "' (boolean expected, got " <<
			luaL_typename(l, -1) << ")").c_str()
			);
		}*/
		value = lua_toboolean(l, -1);
	}

	lua_pop(l, 1);
	return value;
}

/**
* @brief A loader that makes require() able to load Lua files
* from the quest data directory or archive.
* @param l The Lua context.
* @return Number of values to return to Lua.
*/
int LuaContext::l_loader(lua_State* l)
{
	const std::string& script_name = luaL_checkstring(l, 1);
    bool exists = load_file_if_exists(l, script_name);

    if(!exists) 
	{
      std::cerr << "No quest file.\n";
    }
    return 1;
}

/**
* @brief Releases a Lua reference.
* @param ref The Lua reference to free.
*/
void LuaContext::destroy_ref(int ref) 
{
  luaL_unref(l, LUA_REGISTRYINDEX, ref);
}

/**
* @brief Calls a function stored in the registry with a reference and
* releases this reference.
* @param callback_ref Reference of the function to call (if LUA_REFNIL,
* nothing is done).
*/
void LuaContext::do_callback(int callback_ref) 
{
  if (callback_ref != LUA_REFNIL) 
  {
    push_callback(callback_ref);
    call_function(0, 0, "callback");
    destroy_ref(callback_ref);
  }
}

/**
* @brief Pushes onto the stack a function stored as a Lua ref.
* @param callback_ref Reference of the function to call (must be
* a valid ref).
*/
void LuaContext::push_callback(int callback_ref) 
{
  push_ref(l, callback_ref);
  if (!lua_isfunction(l, -1)) 
  {
    std::cout << "No such Lua callback\n";
  }
}

/**
* @brief Releases the reference to a Lua callback.
*
* The callback may then be collected by Lua.
*
* @param callback_ref reference of the function (if LUA_REFNIL,
* nothing is done)
*/
void LuaContext::cancel_callback(int callback_ref) 
{
  destroy_ref(callback_ref);
}

/**
* @brief Gets a method of the object on top of the stack.
*
* This is equivalent to find_method(-1, function_name).
*
* @param function_name Name of the function to find in the object.
* @return true if the function was found.
*/
bool LuaContext::find_method(const std::string& function_name)
{
	return find_method(-1, function_name);
}

/**
* @brief Gets a method of an object.
*
* If the method exists, the method and the object are both pushed
* so that you can call the method immediately with the object as first parameter.
* If the method is not found, the stack is left unchanged.
*
* @param index Index of the object in the stack.
* @param function_name Name of the function to find in the object.
* @return true if the function was found.
*/
bool LuaContext::find_method(int index, const std::string& function_name)
{
	index = get_positive_index(l, index);

	lua_getfield(l, index, function_name.c_str());

	bool exists = lua_isfunction(l, -1);
	if(exists)
	{
		lua_pushvalue(l, index);
	}
	else
	{
		lua_pop(l, 1);
	}
	return exists;
}
/**
* @brief Opens a script if it exists and lets it on top of the stack as a
* function.
*
* If the file does not exist, the stack is left intact and false is returned.
*
* @param l A Lua state.
* @param script_name File name of the script with or without extension,
* relative to the data directory.
* @return true if the file exists and was loaded.
*/
bool LuaContext::load_file_if_exists(lua_State* l, const std::string& script_name)
{
	std::string file_name(script_name);

	if(!FileTools::data_file_exists(file_name))
	{
		std::ostringstream oss;
		oss << script_name << ".lua";
		file_name = oss.str();

		if(FileTools::data_file_exists(file_name))
		{
			size_t size;
			char* buffer;
			FileTools::data_file_open_buffer(file_name, &buffer, &size);
			int result = luaL_loadbuffer(l, buffer, size, file_name.c_str());
			FileTools::data_file_close_buffer(buffer);
			
			if(result != 0)
			{
				std::cerr << "Failed to load script.\n";
			}
			return true;
		}
		return false;
	}
	  
}

bool LuaContext::do_file_if_exists(lua_State* l, const std::string& script_name)
{
	if(load_file_if_exists(l, script_name))
	{
		call_function(l, 0, 0, script_name);
		return true;
	}
	return false;
}

/**
* @brief For an index in the Lua stack, returns an equivalent positive index.
* @param l A Lua state.
* @param index An index in the stack (positive or negative, but not a pseudo-index).
* @return The corresponding positive index.
*/
int LuaContext::get_positive_index(lua_State* l, int index)
{
	int positive_index = index;
	if(index < 0)
	{
		positive_index = lua_gettop(l) + index + 1;
	}
	return positive_index;
}

/**
* @brief Calls the Lua function with its arguments on top of the stack.
*
* This function is like lua_pcall, except that it additionaly handles the
* error message if an error occurs in the Lua code (the error is printed).
* This function leaves the results on the stack if there is no error,
* and leaves nothing on the stack in case of error.
*
* @param nb_arguments number of arguments placed on the Lua stack above the
* function to call
* @param nb_results number of results expected (you get them on the stack if
* there is no error)
* @param function_name a name describing the Lua function (only used to print
* the error message if any)
* @return true in case of success
*/
bool LuaContext::call_function(int nb_arguments, int nb_results, const std::string& function_name)
{
	return call_function(l, nb_arguments, nb_results, function_name);
}

/**
* @brief Calls the Lua function with its arguments on top of the stack.
*
* This function is like lua_pcall, except that it additionaly handles the
* error message if an error occurs in the Lua code (the error is printed).
* This function leaves the results on the stack if there is no error,
* and leaves nothing on the stack in case of error.
*
* @param l A Lua state.
* @param nb_arguments Number of arguments placed on the Lua stack above the
* function to call.
* @param nb_results Number of results expected (you get them on the stack if
* there is no error).
* @param function_name A name describing the Lua function (only used to print
* the error message if any).
* @return true in case of success.
*/
bool LuaContext::call_function(lua_State* l, int nb_arguments, int nb_results, const std::string& function_name)
{
	
	if(lua_pcall(l, nb_arguments, nb_results, 0) != 0)
	{
		std::cerr << "Error in function " << function_name << '\n';
		std::cerr << lua_tostring(l, -1) << std::endl;
		lua_pop(l, 1);
		return false;
	}
	return true;
}



/**
* @brief Calls the on_started() method of the object on top of the stack.
*/
void LuaContext::on_started()
{
	if(find_method("on_started"))
	{
		call_function(1, 0, "on_started");
	}
}

/**
* \brief Calls the on_finished() method of the object on top of the stack.
*/
void LuaContext::on_finished() 
{
  if (find_method("on_finished")) 
  {
    call_function(1, 0, "on_finished");
  }
}

/**
* \brief Calls the on_closed() method of the object on top of the stack.
*/
void LuaContext::on_closed() 
{
  if (find_method("on_closed")) 
  {
    call_function(1, 0, "on_closed");
  }
}

/**
* @brief Calls the on_draw() method of the object on top of the stack.
* @param dst_surface The destination surface.
*/
void LuaContext::on_draw(Surface& dst_surface) 
{
  if (find_method("on_draw")) 
  {
    push_surface(l, dst_surface);
    call_function(2, 0, "on_draw");
  }
}

/**
* @brief Calls an input callback method of the object on top of the stack.
* @param event The input event to forward.
* @return \c true if the event was handled and should stop being propagated.
*/
bool LuaContext::on_input(InputEvent& event) 
{
  // Call the Lua function(s) corresponding to this input event.
  bool handled = false;
  if (event.is_keyboard_event()) 
  {
    // Keyboard.
    if (event.is_keyboard_key_pressed()) 
	{
      handled = on_key_pressed(event) || handled;/*
      if (event.is_character_pressed()) 
	  {
        handled = on_character_pressed(event) || handled;
      }*/
    }
    /*
	else if (event.is_keyboard_key_released()) 
	{
      handled = on_key_released(event) || handled;
    }
	*/
  }
  //TODO Change this to mouse input events 
  /*
  else if (event.is_joypad_event()) 
  {
    // Joypad.
    if (event.is_joypad_button_pressed()) {
      handled = on_joypad_button_pressed(event) || handled;
    }
    else if (event.is_joypad_button_released()) {
      handled = on_joypad_button_released(event) || handled;
    }
    else if (event.is_joypad_axis_moved()) {
      handled = on_joypad_axis_moved(event) || handled;
    }
    else if (event.is_joypad_hat_moved()) {
      handled = on_joypad_hat_moved(event) || handled;
    }
  }*/

  return handled;
}

/**
* @brief Notifies the object on top of the stack
* that a keyboard key was just pressed
* (including if it is a directional key or a character).
* @param event The corresponding input event.
* @return \c true if the event was handled and should stop being propagated.
*/
bool LuaContext::on_key_pressed(InputEvent& event) 
{
  bool handled = false;
  if (find_method("on_key_pressed")) 
  {
    const std::string& key_name = InputEvent::get_keyboard_key_name(event.get_keyboard_key());
    if (!key_name.empty()) 
	{ 
	  // This key exists in the KQ API.
      push_string(l, key_name);
      lua_newtable(l);
	  /*
      if (event.is_with_shift()) 
	  {
        lua_pushboolean(l, 1);
        lua_setfield(l, -2, "shift");
      }

      if (event.is_with_control()) 
	  {
        lua_pushboolean(l, 1);
        lua_setfield(l, -2, "control");
      }

      if (event.is_with_alt()) 
	  {
        lua_pushboolean(l, 1);
        lua_setfield(l, -2, "alt");
      }
	  */
      bool success = call_function(3, 1, "on_key_pressed");
      if (!success) 
	  {
        // Something was wrong in the script: don't propagate the input to other objects.
        handled = true;
      }
      else 
	  {
        handled = lua_toboolean(l, -1);
        lua_pop(l, 1);
      }
    }
    else 
	{
      // The method exists but the key is unknown.
      lua_pop(l, 2); // Pop the object and the method.
    }
  }
  return handled;
}

/**
* @brief Creates a reference to the Lua value on top of the stack.
* @return The reference created.
*/
int LuaContext::create_ref() 
{
  return luaL_ref(l, LUA_REGISTRYINDEX);
}