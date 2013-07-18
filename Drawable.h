/** @file Drawable.h */

#ifndef KQ_DYNAMIC_DISPLAYABLE_H
#define KQ_DYNAMIC_DISPLAYABLE_H

#include "Common.h"
#include "ExportableToLua.h"
#include "Rectangle.h"
#include "LuaContext.h"
#include "lua.hpp"
#include "Transition.h"


/** @brief Needs Movement */

class Drawable: public ExportableToLua
{
private:
	Rectangle xy;				/**< Current position of this  (result of movements). */
								
    //Movement* movement;		/**< A movement applied, or NULL (will be deleted then if unused elsewhere). */
	Transition* transition;		/**< A transition applied or NULL */
	int transition_callback_ref;/**< Lua registry ref of a function to call when the transition finishes. */
    LuaContext* lua_context;	/**< The Lua world used for callbacks. */

	//const std::string& get_lua_type_name();

protected:
	Drawable();

public:
	virtual ~Drawable();

    // dynamic effects
    //void start_movement(Movement& movement);
    //void stop_movement();
    //Movement* get_movement();
    const Rectangle& get_xy();
    void set_xy(const Rectangle& xy);

	void start_transition(Transition& transition, int callback_ref, LuaContext* lua_context);
	void stop_transition();
	Transition* get_transition();

	//drawing with effects
	void draw(Surface& dst_surface);
    void draw(Surface& dst_surface, int x, int y);
    void draw(Surface& dst_surface, const Rectangle& dst_position);
    void draw_region(const Rectangle& region, Surface& dst_surface, const Rectangle& dst_position);

	/**
	* @brief Draws this object without applying dynamic effects.
	*
	* Redefine this function to draw your object onto the destination
	* surface.
	*
	* @param dst_surface The destination surface.
	* @param dst_position Coordinates on the destination surface.
	*/
    virtual void raw_draw(Surface& dst_surface, const Rectangle& dst_position) = 0;

	/**
	* @brief Draws a subrectangle of this object without applying dynamic
	* effects.
	*
	* Redefine this function to draw your object onto the destination
	* surface.
	*
	* @param region The subrectangle to draw in this object.
	* @param dst_surface The destination surface.
	* @param dst_position Coordinates on the destination surface.
	*/
    virtual void raw_draw_region(const Rectangle& region, Surface& dst_surface, const Rectangle& dst_position) = 0;

	/**
	* @brief Draws a transition effect on this drawable object.
	*
	* Redefine this function to apply the transition effect on the surface
	* of your object.
	*
	* @param transition The transition effect to apply.
	*/
	virtual void draw_transition(Transition& transition) = 0;
	virtual void update();
};

#endif
