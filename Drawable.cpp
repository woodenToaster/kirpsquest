/** @file Drawable.cpp */

#include "Surface.h"
#include "Transition.h"
#include "TransitionFade.h"
#include "LuaContext.h"
#include "lua.hpp"

Drawable::Drawable(): xy(), transition(NULL), transition_callback_ref(LUA_REFNIL), lua_context(NULL)
{
}

Drawable::~Drawable()
{
	stop_transition();
	//stop movement();
}

//Implement Movement functions

/**
* @brief Returns the coordinates of this drawable object as defined by its
* movement.
* @return The coordinates of this drawable object.
*/
const Rectangle& Drawable::get_xy() 
{
  return xy;
}

/**
* @brief Sets the coordinates of this drawable object.
* @param xy The new coordinates of this drawable object.
*/
void Drawable::set_xy(const Rectangle& xy) 
{
  this->xy.set_xy(xy);
}

/**
* @brief Starts a transition effect on this object.
*
* The transition will be automatically deleted when finished or stopped.
* Any previous transition is stopped.
*
* @param transition The transition to start.
* @param callback_ref A Lua registry ref to the function to call when
* the transition finishes, or LUA_REFNIL.
* @param lua_context The Lua world for the callback (or NULL).
*/
void Drawable::start_transition(Transition& transition, int callback_ref, LuaContext* lua_context) 
{
  stop_transition();
  
  this->transition = &transition;
  this->transition_callback_ref = callback_ref;
  this->lua_context = lua_context;
  transition.start();
}

/**
* @brief Stops the transition effect applied to this object, if any.
*
* The transition is deleted and the Lua callback (if any) is canceled.
*/
void Drawable::stop_transition() 
{
  delete transition;
  transition = NULL;

  if (lua_context != NULL) 
  { 
    lua_context->cancel_callback(this->transition_callback_ref);
    transition_callback_ref = LUA_REFNIL;
  }
}

/**
* @brief Updates this object.
*
* This function is called repeatedly.
* You can redefine it for your needs.
*/
void Drawable::update() 
{
  if (transition != NULL) 
  {
    transition->update();
    if (transition->is_finished()) 
	{
		if(lua_context != NULL)
		{
			int ref = transition_callback_ref;
			transition_callback_ref = LUA_REFNIL;
			lua_context->do_callback(ref);
		}
		stop_transition();
    }
  }

  /**
  if (movement != NULL) 
  {
    movement->update();
    if (movement != NULL && movement->is_finished()) 
	{
      stop_movement();
    }
  }*/
}

/**
* @brief Draws this object, applying dynamic effects.
* @param dst_surface the destination surface
*/
void Drawable::draw(Surface& dst_surface)
{
	draw(dst_surface, Rectangle(0, 0));
}

/**
* @brief Draws this object, applying dynamic effects.
* @param dst_surface the destination surface
* @param x x coordinate of where to draw
* @param y y coordinate of where to draw
*/
void Drawable::draw(Surface& dst_surface, int x, int y) 
{
  draw(dst_surface, Rectangle(x, y));
}

/**
* @brief Draws this object, applying dynamic effects.
* @param dst_surface the destination surface
* @param dst_position position on this surface
* (will be added to the position obtained by previous movements)
*/
void Drawable::draw(Surface& dst_surface, const Rectangle& dst_position) 
{
	Rectangle dst_position2(dst_position);
	dst_position2.add_xy(xy);
	
	if(transition != NULL)
	{
		draw_transition(*transition);
	}

	raw_draw(dst_surface, dst_position2);
}

/**
* @brief Draws a subrectangle of this object, applying dynamic effects.
* @param region The rectangle to draw in this object.
* @param dst_surface The destination surface
* @param dst_position Position on this surface
* (will be added to the position obtained by previous movements).
*/
void Drawable::draw_region(const Rectangle& region, Surface& dst_surface, const Rectangle& dst_position) 
{
  Rectangle dst_position2(dst_position);
  dst_position2.add_xy(xy);
  raw_draw_region(region, dst_surface, dst_position2);
}

//trying to appease the compiler
/*
const std::string& Drawable::get_lua_type_name()
{
	return LuaContext::surface_module_name;
}*/
