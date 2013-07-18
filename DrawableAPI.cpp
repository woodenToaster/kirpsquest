/** @file DrawableAPI.cpp */

#include "LuaContext.h"
#include "Drawable.h"
#include "lua.hpp"
#include "TransitionFade.h"

/**
* @brief Returns whether a value is a userdata of a type.
* @param l A Lua context.
* @param index An index in the stack.
* @return true if the value at this index is a drawable.
*/
bool LuaContext::is_drawable(lua_State* l, int index) 
{
  return true; //TODO: Implement later: is_surface(l, index) || is_text_surface(l, index) || is_sprite(l, index);
}

/**
* @brief Check that the userdata at the specified index is a drawable
* object (surface, text surface or sprite) and returns it.
* @param l a Lua context
* @param index an index in the stack
* @return the surface
*/
Drawable& LuaContext::check_drawable(lua_State* l, int index) 
{
  Drawable** drawable = NULL;

  if (is_drawable(l, index)) 
  {
    drawable = static_cast<Drawable**>(lua_touserdata(l, index));
  }
  else 
  {
    luaL_typerror(l, index, "drawable");
  }

  return **drawable;
}

/**
* @brief Returns whether a drawable object was created by this script.
* @param drawable A drawable object.
* @return true if the drawable object was created by this script.
*/
bool LuaContext::has_drawable(Drawable* drawable) 
{
  return drawables.find(drawable) != drawables.end();
}
/**
* @brief Registers a drawable object created by this script.
* @param drawable a drawable object
*/
void LuaContext::add_drawable(Drawable* drawable) 
{
   drawables.insert(drawable);
}

/**
* @brief Unregisters a drawable object created by this script.
* @param drawable a drawable object
*/
void LuaContext::remove_drawable(Drawable* drawable) 
{
  drawables.erase(drawable);
}

/**
* \brief Destroys from Lua all drawable objects created
* by this script.
*/
void LuaContext::destroy_drawables() 
{
  std::set<Drawable*>::iterator it;
  for (it = drawables.begin(); it != drawables.end(); ++it) 
  {
    Drawable* drawable = *it;
    drawable->decrement_refcount();
    if (drawable->get_refcount() == 0) 
	{
      delete drawable;
    }
  }
  drawables.clear();
  drawables_to_remove.clear();
}

/**
* @brief Finalizer of types sprite, surface and text surface.
* @param l the Lua context that is calling this function
* @return number of values to return to Lua
*/
int LuaContext::drawable_meta_gc(lua_State* l) 
{
  LuaContext& lua_context = get_lua_context(l);
  Drawable& drawable = check_drawable(l, 1);

  if (drawable.get_refcount() == 1) {
    lua_context.remove_drawable(&drawable);
  }
  userdata_meta_gc(l);

  return 0;
}

/**
* @brief Updates all drawable objects created by this script.
*/
void LuaContext::update_drawables() 
{
  std::set<Drawable*>::iterator it;
  for(it = drawables.begin(); it != drawables.end(); it++) 
  {
	  Drawable* drawable = *it;
	  if(has_drawable(drawable))
	  {
		  drawable->update();
	  }
  }

  //Remove the ones that should be removed
  for(it = drawables_to_remove.begin(); it != drawables_to_remove.end(); ++it)
  {
	  Drawable* drawable = *it;
	  drawables.erase(drawable);
	  drawable->decrement_refcount();
	  if(drawable->get_refcount() == 0)
	  {
		  delete drawable;
	  }
  }
  drawables_to_remove.clear();
}

/**
* @brief Implementation of drawable:draw().
* @param l the Lua context that is calling this function
* @return number of values to return to Lua
*/
int LuaContext::drawable_api_draw(lua_State* l) 
{
	Drawable& drawable = check_drawable(l, 1);
	Surface& dst_surface = check_surface(l, 2);
	int x = luaL_optint(l, 3, 0);
	int y = luaL_optint(l, 4, 0);
	drawable.draw(dst_surface, x, y);

	return 0;
}

/**
* @brief Implementation of drawable:fade_out().
* @param l The Lua context that is calling this function.
* @return Number of values to return to Lua.
*/
int LuaContext::drawable_api_fade_out(lua_State* l) 
{
  uint32_t delay = 20;
  int callback_ref = LUA_REFNIL;

  Drawable& drawable = check_drawable(l, 1);

  if (lua_gettop(l) >= 2) 
  {
    // the second argument can be the delay or the callback
    int index = 2;
    if (lua_isnumber(l, index)) 
	{
      delay = lua_tonumber(l, index);
      index++;
    }
    // the next argument (if any) is the callback
    if (lua_gettop(l) >= index) 
	{
      luaL_checktype(l, index, LUA_TFUNCTION);
      lua_settop(l, index);
      callback_ref = luaL_ref(l, LUA_REGISTRYINDEX);
    }
  }

  TransitionFade* transition = new TransitionFade(Transition::OUT);
  transition->set_delay(delay);
  drawable.start_transition(*transition, callback_ref, &get_lua_context(l));
 
  return 0;
}