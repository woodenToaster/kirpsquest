/** @file MenuAPI.cpp */

#include "LuaContext.h"
#include "Surface.h"
#include "lua.hpp"
#include <list>

const std::string LuaContext::menu_module_name = "kq.menu";

void LuaContext::register_menu_module()
{
	static const luaL_Reg functions[] =
	{
		{ "start", menu_api_start },/*
		{ "stop", menu_api_stop },
		{ "stop_all", menu_api_stop_all },*/
		{ NULL, NULL }
	};
	register_functions(menu_module_name, functions);
}

/**
* @brief Registers a menu into a context (table or a userdata).
*
* This function can be called safely even while iterating on the menus list.
*
* @param menu_ref Lua ref of the menu to add.
* @param context_index Index of the table or userdata in the stack.
*/
void LuaContext::add_menu(int menu_ref, int context_index) 
{
	const void* context;
	if (lua_type(l, context_index) == LUA_TUSERDATA) 
	{
		ExportableToLua** userdata = static_cast<ExportableToLua**>(lua_touserdata(l, context_index));
		context = *userdata;
	}
	else 
	{
		context = lua_topointer(l, context_index);
	}

	menus.push_back(LuaMenuData(menu_ref, context));
	
	menu_on_started(menu_ref);
}

/**
* \brief Unregisters all menus associated to a context.
*
* This function can be called safely even while iterating on the menus list.
*
* \param context_index Index of a table or userdata containing menus.
*/
void LuaContext::remove_menus(int context_index) 
{
  const void* context;
  if (lua_type(l, context_index) == LUA_TUSERDATA) 
  {
    ExportableToLua** userdata = static_cast<ExportableToLua**>(lua_touserdata(l, context_index));
    context = *userdata;
  }
  else 
  {
    context = lua_topointer(l, context_index);
  }

  std::list<LuaMenuData>::iterator it;
  for (it = menus.begin(); it != menus.end(); ++it) 
  {
    int menu_ref = it->ref;
    if (it->context == context) 
	{
      menu_on_finished(menu_ref);
      destroy_ref(menu_ref);
      it->ref = LUA_REFNIL;
      it->context = NULL;
    }
  }
}

/**
* \brief Unregisters all existing menus.
*
* This function can be called safely even while iterating on the menus list.
*/
void LuaContext::remove_menus() 
{
  std::list<LuaMenuData>::iterator it;
  for (it = menus.begin(); it != menus.end(); ++it) 
  {
    int menu_ref = it->ref;
    if (menu_ref != LUA_REFNIL) 
	{
      menu_on_finished(menu_ref);
      destroy_ref(menu_ref);
      it->ref = LUA_REFNIL;
      it->context = NULL;
    }
  }
}

/**
* \brief Destroys immediately all existing menus.
*/
void LuaContext::destroy_menus() 
{
  std::list<LuaMenuData>::iterator it;
  for (it = menus.begin(); it != menus.end(); ++it) 
  {
    int menu_ref = it->ref;
    if (menu_ref != LUA_REFNIL) 
	{
      destroy_ref(menu_ref);
    }
  }
  menus.clear();
}

/**
* @brief Checks all menus and removes the ones that have to be removed.
*
* Note that the on_update() is called by the context of each menu, not
* by this function.
*/
void LuaContext::update_menus() 
{
  // Destroy the ones that should be removed.
  std::list<LuaMenuData>::iterator it;
  for (it = menus.begin(); it != menus.end(); ++it) {

    if (it->ref == LUA_REFNIL) 
	{
      // LUA_REFNIL on a menu means that we should remove it.
      // In this case, context must also be NULL.
      //Debug::check_assertion(it->context == NULL, "Menu with context and no ref");
      menus.erase(it--);
    }
  }
}

/**
* @brief Implementation of kq.menu.start().
* @param l The Lua context that is calling this function.
* @return Number of values to return to Lua.
*/
int LuaContext::menu_api_start(lua_State *l) {

  // Parameters: context table.
  if (lua_type(l, 1) != LUA_TTABLE
      && lua_type(l, 1) != LUA_TUSERDATA) {
    luaL_typerror(l, 1, "table or userdata");
  }
  luaL_checktype(l, 2, LUA_TTABLE);
  lua_settop(l, 2);

  LuaContext& lua_context = get_lua_context(l);
  int menu_ref = lua_context.create_ref();
  lua_context.add_menu(menu_ref, 1);

  return 0;
}

void LuaContext::menu_on_started(int menu_ref)
{
	push_ref(l, menu_ref);
	on_started();
	lua_pop(l, 1);
}

/**
* \brief Calls the on_finished() method of a Lua menu.
* \param menu_ref A reference to the menu object.
*/
void LuaContext::menu_on_finished(int menu_ref) 
{
  push_ref(l, menu_ref);
  on_finished();
  remove_timers(-1); // Stop timers associated to this menu.
  lua_pop(l, 1);
}

/**
* @brief Calls the on_draw() method of a Lua menu.
* @param menu_ref A reference to the menu object.
* @param dst_surface The destination surface.
*/
void LuaContext::menu_on_draw(int menu_ref, Surface& dst_surface) 
{
  push_ref(l, menu_ref);
  on_draw(dst_surface);
  lua_pop(l, 1);
}

/**
* @brief Calls an input callback method of a Lua menu.
* @param menu_ref A reference to the menu object.
* @param event The input event to forward.
* @return \c true if the event was handled and should stop being propagated.
*/
bool LuaContext::menu_on_input(int menu_ref, InputEvent& event) 
{
  // Get the Lua menu.
  push_ref(l, menu_ref);

  // Trigger its appropriate callback if it exists.
  bool handled = on_input(event);

  // Remove the menu from the stack.
  lua_pop(l, 1);

  return handled;
}

/**
* @brief Calls the on_draw() method of the menus associated to a context.
* @param context_index Index of an object with menus.
* @param dst_surface The destination surface to draw.
*/
void LuaContext::menus_on_draw(int context_index, Surface& dst_surface) 
{
  const void* context;
  if (lua_type(l, context_index) == LUA_TUSERDATA) 
  {
    ExportableToLua** userdata = static_cast<ExportableToLua**>(lua_touserdata(l, context_index));
    context = *userdata;
  }
  else 
  {
    context = lua_topointer(l, context_index);
  }

  std::list<LuaMenuData>::iterator it;
  for (it = menus.begin(); it != menus.end(); ++it) 
  {
    int menu_ref = it->ref;
    if (it->context == context) 
	{
      menu_on_draw(menu_ref, dst_surface);
    }
  }
}

/**
* @brief Calls the on_input() method of the menus associated to a context.
* @param context_index Index of an object with menus.
* @param event The input event to handle.
* @return \c true if the event was handled and should stop being propagated.
*/
bool LuaContext::menus_on_input(int context_index, InputEvent& event) 
{
  const void* context;
  if (lua_type(l, context_index) == LUA_TUSERDATA) 
  {
    ExportableToLua** userdata = static_cast<ExportableToLua**>(lua_touserdata(l, context_index));
    context = *userdata;
  }
  else 
  {
    context = lua_topointer(l, context_index);
  }

  bool handled = false;
  std::list<LuaMenuData>::reverse_iterator it;
  for (it = menus.rbegin(); it != menus.rend() && !handled; ++it) 
  {
    int menu_ref = it->ref;
    if (it->context == context) 
	{
      handled = menu_on_input(menu_ref, event);
    }
  }

  return handled;
}

