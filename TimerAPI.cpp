/** @file TimerAPI.cpp */

#include "LuaContext.h"
#include "Timer.h"

const std::string LuaContext::timer_module_name = "kq.timer";

void LuaContext::register_timer_module()
{
	//Functions of kq.timer
	static const luaL_Reg functions[] =
	{
		{ "start", timer_api_start },
		//{ "stop_all", timer_api_stop_all },
		{ NULL, NULL }
	};
	register_functions(timer_module_name, functions);

	//Methods of the timer type
	static const luaL_Reg methods[] = 
	{/*
      { "stop", timer_api_stop },
      { "is_with_sound", timer_api_is_with_sound },
      { "set_with_sound", timer_api_set_with_sound },
      { "is_suspended", timer_api_is_suspended },
      { "set_suspended", timer_api_set_suspended },
      { "is_suspended_with_map", timer_api_is_suspended_with_map },
      { "set_suspended_with_map", timer_api_set_suspended_with_map },*/
      { NULL, NULL }
    };
    static const luaL_Reg metamethods[] = 
	{
      { "__gc", userdata_meta_gc },
      { NULL, NULL }
    };
    register_type(timer_module_name, methods, metamethods);

}

/**
* @brief Pushes a timer userdata onto the stack.
* @param l a Lua context
* @param timer a timer
*/
void LuaContext::push_timer(lua_State* l, Timer& timer)
{
	push_userdata(l, timer);
}

/**
* @brief Registers a timer into a context (table or a userdata).
* @param timer A timer.
* @param context_index Index of the table or userdata in the stack.
* @param callback_index Index of the function to call when the timer finishes.
*/
void LuaContext::add_timer(Timer* timer, int context_index, int callback_index)
{
	const void* context;
	if(lua_type(l, context_index) == LUA_TUSERDATA)
	{
		ExportableToLua** userdata = static_cast<ExportableToLua**>(lua_touserdata(l, context_index));
		context = *userdata;
	}
	else
	{
		context = lua_topointer(l, context_index);
	}
	lua_pushvalue(l, callback_index);
	int callback_ref = create_ref();

	timers[timer].callback_ref = callback_ref;
    timers[timer].context = context;

	/*
	Game* game = main_loop.get_game();
	if(game != NULL)
	{
		//We are in a game: depending on the timer's cotext,
		//when the map is suspended, also suspend the timer or not.
		if(is_map(l, context_index) || is_entity(l, context_index) || is_item(l, context_index))
		{
			timer->set_is_suspended_with_map(true);
		}
	}
	*/
	timer->increment_refcount();
}

/**
* \brief Unregisters a timer associated to a context.
*
* This function can be called safely even while iterating on the timer list.
*
* \param timer A timer.
*/
void LuaContext::remove_timer(Timer* timer) 
{
  if (timers.find(timer) != timers.end()) 
  {
    if (!timer->is_finished()) 
	{
      cancel_callback(timers[timer].callback_ref);
    }
    timers[timer].callback_ref = LUA_REFNIL;
    timers_to_remove.push_back(timer);
  }
}

/**
* \brief Unregisters all timers associated to a context.
*
* This function can be called safely even while iterating on the timer list.
*
* \param context_index Index of a table or userdata containing timers.
*/
void LuaContext::remove_timers(int context_index) 
{
  std::list<Timer*> timers_to_remove;

  const void* context;
  if (lua_type(l, context_index) == LUA_TUSERDATA) 
  {
    ExportableToLua** userdata = (ExportableToLua**) lua_touserdata(l, context_index);
    context = *userdata;
  }
  else 
  {
    context = lua_topointer(l, context_index);
  }

  std::map<Timer*, LuaTimerData>::iterator it;
  for (it = timers.begin(); it != timers.end(); ++it) 
  {
    Timer* timer = it->first;
    if (it->second.context == context) 
	{
      if (!timer->is_finished()) 
	  {
        destroy_ref(it->second.callback_ref);
      }
      it->second.callback_ref = LUA_REFNIL;
      timers_to_remove.push_back(timer);
    }
  }
}

/**
* \brief Destroys immediately all existing timers.
*/
void LuaContext::destroy_timers() 
{
  std::map<Timer*, LuaTimerData>::iterator it;
  for (it = timers.begin(); it != timers.end(); ++it) 
  {
    Timer* timer = it->first;
    if (!timer->is_finished()) 
	{
      destroy_ref(it->second.callback_ref);
    }
    timer->decrement_refcount();
    if (timer->get_refcount() == 0) 
	{
      delete timer;
    }
  }
  timers.clear();
}

/**
* @brief Updates all timers currently running for this script.
*/
void LuaContext::update_timers() 
{
  // Update all timers.
  std::map<Timer*, LuaTimerData>::iterator it;
  for (it = timers.begin(); it != timers.end(); ++it) 
  {
    Timer* timer = it->first;
    timer->update();
    if (timer->is_finished()) 
	{
      do_callback(it->second.callback_ref);
      it->second.callback_ref = LUA_REFNIL;
      timers_to_remove.push_back(timer);
    }
  }

  // Destroy the ones that should be removed.
  std::list<Timer*>::iterator it2;
  for (it2 = timers_to_remove.begin(); it2 != timers_to_remove.end(); ++it2) 
  {
    Timer* timer = *it2;
    if (timers.find(timer) != timers.end()) 
	{
      if (!timer->is_finished()) 
	  {
        cancel_callback(timers[timer].callback_ref);
      }
      timers.erase(timer);
      timer->decrement_refcount();
      if (timer->get_refcount() == 0) 
	  {
        delete timer;
      }
    }
  }
  timers_to_remove.clear();
}

int LuaContext::timer_api_start(lua_State* l)
{
	//Parameters: [context] delay callback
	LuaContext& lua_context = get_lua_context(l);

	if(lua_type(l, 1) != LUA_TNUMBER)
	{
		//The first parameter is the context
		if(lua_type(l, 1) != LUA_TTABLE && lua_type(l, 1) != LUA_TUSERDATA)
		{
			luaL_typerror(l, 1, "table or userdata");
		}
	}
	else
	{
		//No context specified: set a default context:
		// - during a game: the current map,
		// - outside a game: kq.main.
		/*
		Game* game = lua_context.get_main_loop().get_game();
		if (game != NULL) 
		{
			push_map(l, game->get_current_map());
		}
		else 
		{
			LuaContext::push_main(l);
		}

		lua_insert(l, 1);*/
	}
	//Now the first parameter is the context

	uint32_t delay = uint32_t(luaL_checkint(l, 2));
	luaL_checktype(l, 3, LUA_TFUNCTION);
	

	if(delay == 0)
	{
		//The delay is zero: call the function right now
		lua_settop(l,3);
		lua_context.call_function(0,0, "callback");
		lua_pushnil(l);
	}
	else
	{
		//Create the timer.
		Timer* timer = new Timer(delay);
		lua_context.add_timer(timer, 1, 3);
		push_timer(l, *timer);
	}
	return 1;
	
}