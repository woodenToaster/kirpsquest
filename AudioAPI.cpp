/** @file AudioAPI.cpp */

#include "LuaContext.h"
#include "Sound.h"
#include "lua.hpp"

const std::string LuaContext::audio_module_name = "kq.audio";

/** @brief Initializes the audio features provided to Lua */

void LuaContext::register_audio_module()
{
	static const luaL_Reg functions[] =
	{
		{ "play_sound", audio_api_play_sound },
        { "preload_sounds", audio_api_preload_sounds },
        //{ "play_music", audio_api_play_music },
        //{ "stop_music", audio_api_stop_music },
        //{ "get_sound_volume", audio_api_get_sound_volume },
        //{ "set_sound_volume", audio_api_set_sound_volume },
        //{ "get_music_volume", audio_api_get_music_volume },
        //{ "set_music_volume", audio_api_set_music_volume },
        { NULL, NULL }
	};
	register_functions(audio_module_name, functions);
}

/**
* @brief Implementation of kq.audio.play_sound().
* @param l the Lua context that is calling this function
* @return number of values to return to Lua
*/
int LuaContext::audio_api_play_sound(lua_State* l) 
{
  const std::string& sound_id = luaL_checkstring(l, 1);

  if (!Sound::exists(sound_id)) 
  {
	  std::string error_msg = "Cannot find sound " + sound_id + "\n";
      luaL_error(l, error_msg.c_str());
  }

  Sound::play(sound_id);
  return 0;
}

/**
* @brief Implementation of kq.audio.preload_sounds().
* @param l the Lua context that is calling this function
* @return number of values to return to Lua
*/
int LuaContext::audio_api_preload_sounds(lua_State* l) 
{
	Sound::load_all();
	return 0;
}