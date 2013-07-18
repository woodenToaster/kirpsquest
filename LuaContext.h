/** @file LuaContext.h */

#ifndef KQ_LUA_CONTEXT_H
#define KQ_LUA_CONTEXT_H

#include "Common.h"
#include "lua.hpp"
#include "MainLoop.h"
#include <map>
#include <list>
#include <set>

class MainLoop;

class LuaContext
{
public:

	//functions and types
	static const std::string main_module_name;		/**< kq.main */
	static const std::string surface_module_name;	/**< kq.surface */
	static const std::string menu_module_name;		/**< kq.menu */
	static const std::string timer_module_name;		/**< kq.timer */
	static const std::string game_module_name;		/**< kq.game */
	static const std::string audio_module_name;		/**< kq.audio */
	static const std::string text_surface_module_name; /**< kq.text_surface */

	LuaContext(MainLoop& main_loop);
    ~LuaContext();

    static LuaContext& get_lua_context(lua_State* l);

    MainLoop& get_main_loop();

	//Main loop from C++
	void initialize();
	void exit();
	void update();
	bool notify_input(InputEvent& event);
    void notify_map_suspended(Map& map, bool suspended);
    void notify_camera_reached_target(Map& map);
    void notify_dialog_finished(int callback_ref, bool skipped, int answer);
    void run_item(EquipmentItem& item);
    void run_map(Map& map, Destination* destination);
    void run_enemy(Enemy& enemy);

	//Lua helpers
	static bool is_color(lua_State* l, int index);
    static Color check_color(lua_State* l, int index);
	static const std::string check_string_field(lua_State* l, int table_index, const std::string& key);
	static const std::string opt_string_field(lua_State* l, int table_index, const std::string& key, 
		const std::string& default_value);
	static int opt_int_field(lua_State* l, int table_index, const std::string& key, int default_value);
	static bool opt_boolean_field(lua_State* l, int table_index, const std::string& key, bool default_value);

	template<typename E>
    static E check_enum(lua_State* l, int index, const std::string names[]);

	static int get_positive_index(lua_State* l, int index);
	static bool is_valid_lua_identifier(const std::string& name);

	//Lua refs
	int create_ref();
	void destroy_ref(int ref);
	void push_callback(int callback_ref);
	void do_callback(int callback_ref);
	void cancel_callback(int callback_ref);

	//Timers
	void add_timer(Timer* timer, int context_idex, int callback_index);
	void remove_timer(Timer* timer);
    void remove_timers(int context_index);
    void destroy_timers();
    void update_timers();
    void notify_timers_map_suspended(bool suspended);

	//Menus
	void add_menu(int menu_ref, int context_index);
	void remove_menus(int context_index);
	void remove_menus();
	void destroy_menus();
	void update_menus();

	//Drawable objects
	bool has_drawable(Drawable* drawable);
	void add_drawable(Drawable* drawable);
	void remove_drawable(Drawable* drawable);
	void destroy_drawables();
	void update_drawables();

	//Main loop events (kq.main)
	void main_on_started();
	void main_on_finished();
    void main_on_update();
    void main_on_draw(Surface& dst_surface);
    bool main_on_input(InputEvent& event);

	//Menu events
	void menu_on_started(int menu_ref);
	void menu_on_finished(int menu_ref);
    void menu_on_update(int menu_ref);
    void menu_on_draw(int menu_ref, Surface& dst_surface);
    bool menu_on_input(int menu_ref, InputEvent& event);
    //bool menu_on_command_pressed(int menu_ref, GameCommands::Command command);
    //bool menu_on_command_released(int menu_ref, GameCommands::Command command);
    void menus_on_update(int context_index);
    void menus_on_draw(int context_index, Surface& dst_surface);
    bool menus_on_input(int context_index, InputEvent& event);
    //bool menus_on_command_pressed(int context_index, GameCommands::Command command);
    //bool menus_on_command_released(int context_index, GameCommands::Command command);

	//Game events
	void game_on_started(Game& game);
    void game_on_finished(Game& game);
    void game_on_update(Game& game);
    void game_on_draw(Game& game, Surface& dst_surface);
    void game_on_map_changed(Game& game, Map& map);
    void game_on_paused(Game& game);
    void game_on_unpaused(Game& game);
    bool game_on_input(Game& game, InputEvent& event);
    //bool game_on_command_pressed(Game& game, GameCommands::Command command);
    //bool game_on_command_released(Game& game, GameCommands::Command command);

	typedef int (FunctionExportedToLua) (lua_State* l);
	
	static FunctionExportedToLua
	
		//Main API
		main_api_load_file,
		main_api_do_file,
		main_api_reset,
		main_api_exit,
		main_api_is_debug_enabled,
		main_api_get_quest_write_dir,
		main_api_set_quest_write_dir,
		main_api_load_settings,
		main_api_save_settings,
		main_api_get_distance,
		main_api_get_angle,

		//Audio API
		audio_api_play_sound,
		audio_api_preload_sounds,
	  
		//Menu API
		menu_api_start,
		menu_api_stop,
		menu_api_stop_all,
	    
		//Timer API
		timer_api_start,
		timer_api_stop,
		timer_api_stop_all,
		timer_api_is_with_sound,
		timer_api_set_with_sound,
		timer_api_is_suspended,
		timer_api_set_suspended,
		timer_api_is_suspended_with_map,
		timer_api_set_suspended_with_map,
		// TODO add get_remaining_time, set_remaining_time,
		// TODO allow to repeat a timer automatically
		// TODO remove is_with_sound, set_with_sound (do this in pure Lua, possibly with a second timer)
		// TODO game:is_suspended, timer:is/set_suspended_with_map, sprite:get/set_ignore_suspend
		// are the same concept, make these names consistent

		// Drawable API (common to surfaces, text surfaces and sprites).
		drawable_api_draw,
		drawable_api_draw_region,
		drawable_api_fade_in,
		drawable_api_fade_out,
		drawable_api_get_xy,
		drawable_api_set_xy,
		drawable_api_get_movement,
		drawable_api_stop_movement,
		drawable_meta_gc,

		//Surface API
		surface_api_create,
		surface_api_get_size,
		surface_api_fill_color,
		surface_api_get_transparency_color,
		surface_api_set_transparency_color,
		surface_api_set_opacity,

		//Text surface API
		text_surface_api_create,
		text_surface_api_get_horizontal_alignment,
		text_surface_api_set_horizontal_alignment,
		text_surface_api_get_vertical_alignment,
		text_surface_api_set_vertical_alignment,
		text_surface_api_get_font,
		text_surface_api_set_font,
		text_surface_api_get_rendering_mode,
		text_surface_api_set_rendering_mode,
		text_surface_api_get_color,
		text_surface_api_set_color,
		text_surface_api_get_text,
		text_surface_api_set_text,
		text_surface_api_set_text_key,
		text_surface_api_get_size,

		//Game API
		game_api_exists,
		game_api_delete,
		game_api_load,
		game_api_save, // TODO allow to change the file name (e.g. to copy)
		game_api_start,
		game_api_is_started,
		game_api_is_suspended,
		game_api_is_paused,
		game_api_set_paused,
		game_api_get_map,
		game_api_get_value,
		game_api_set_value,
		game_api_get_starting_location,
		game_api_set_starting_location, // TODO don't do it automatically, use on_map_changed
		game_api_get_life,
		game_api_set_life,
		game_api_add_life,
		game_api_remove_life,
		game_api_get_max_life,
		game_api_set_max_life,
		game_api_add_max_life,
		game_api_get_money,
		game_api_set_money,
		game_api_add_money,
		game_api_remove_money,
		game_api_get_max_money,
		game_api_set_max_money,
		game_api_get_magic,
		game_api_set_magic,
		game_api_add_magic,
		game_api_remove_magic,
		game_api_get_max_magic,
		game_api_set_max_magic,
		game_api_has_ability,
		game_api_get_ability,
		game_api_set_ability,
		game_api_get_item,
		game_api_has_item,
		game_api_get_item_assigned,
		game_api_set_item_assigned,
		game_api_is_command_pressed,
		game_api_get_commands_direction,
		game_api_get_command_effect, // TODO also return "run" for action command
		game_api_get_command_keyboard_binding,
		game_api_set_command_keyboard_binding,
		game_api_get_command_joypad_binding,
		game_api_set_command_joypad_binding,
		game_api_capture_command_binding,


		//Available to all userdata types
		userdata_meta_gc,
		userdata_meta_newindex_as_table,
		userdata_meta_index_as_table;

private:

	struct LuaMenuData
	{
		int ref;
		const void* context;		/**< Lua table or userdata the menu is attached to. */

		LuaMenuData(int ref, const void* context): ref(ref), context(context)
		{
		}
	};

	struct LuaTimerData
	{
		int callback_ref;		/**< Lua ref to the function to call after the timer goes off */
		const void* context;	/**< Lua table or userdata the timer is attached to */
	};

	//Functions exported to Lua for internal needs
	static FunctionExportedToLua l_loader;

	lua_State* l;
	MainLoop& main_loop;
	std::list<LuaMenuData> menus;
	std::map<Timer*, LuaTimerData> timers;
	std::list<Timer*> timers_to_remove;
	std::set<Drawable*> drawables;
	std::set<Drawable*> drawables_to_remove;



	static std::map<lua_State*, LuaContext*> lua_contexts;	/**< Mapping to get the encapsulated object from the 
															 *   lua_State pointer. */
	//Executing Lua code.
	bool find_method(int index, const std::string& function_name);
	bool find_method(const std::string& function_name);
	bool call_function(int nb_arguments, int nb_results, const std::string& function_name);
	static bool call_function(lua_State* l, int nb_arguments, int nb_results, const std::string& function_name);
	static void load_file(lua_State* l, const std::string& script_name);
	static bool load_file_if_exists(lua_State* l, const std::string& script_name);
	static void do_file(lua_State* l, const std::string& script_name);
	static bool do_file_if_exists(lua_State* l, const std::string& script_name);

	//Initialization of modules
	void register_functions(const std::string& module_name, const luaL_Reg* functions);
	void register_type(const std::string& module_name, const luaL_Reg* methods, const luaL_Reg* metamethods);
	void register_modules();
    void register_main_module();
	void register_game_module();
	void register_map_module();
	void register_entity_module();
	void register_audio_module();
	void register_timer_module();
	void register_surface_module();
	void register_text_surface_module();
	void register_sprite_module();
	void register_movement_module();
	void register_item_module();
	void register_input_module();
	void register_video_module();
	void register_menu_module();
	void register_language_module();

	//Pushing objects to Lua
	static void push_ref(lua_State* l, int ref);
	static void push_main(lua_State* l);
	static void push_string(lua_State* l, const std::string& text);
	static void push_userdata(lua_State* l, ExportableToLua& userdata);
	static void push_timer(lua_State* l, Timer& timer);
	static void push_surface(lua_State* l, Surface& surface);
	static void push_text_surface(lua_State* l, TextSurface& text_surface);
	static void push_game(lua_State* l, Savegame& game);


	//Getting userdata objects from Lua.
	static bool is_userdata(lua_State* l, int index, const std::string& module_name);
	static ExportableToLua& check_userdata(lua_State* l, int index, const std::string& module_name);
	static bool is_drawable(lua_State* l, int index);
	static Drawable& check_drawable(lua_State* l, int index);
	static bool is_surface(lua_State* l, int index);
	static Surface& check_surface(lua_State* l, int index);
	static bool is_game(lua_State* l, int index);
    static Savegame& check_game(lua_State* l, int index);

	//Events
	void on_started();
	void on_finished();
	void on_update();
	void on_draw(Surface& dst_surface);
	bool on_input(InputEvent& event);
	bool on_key_pressed(InputEvent& event);
	bool on_key_released(InputEvent& event);
	bool on_character_pressed(InputEvent& event);
	void on_closed();

};


/**
* \brief Checks whether a value is the name of an enumeration value and
* returns this value.
*
* Raises a Lua error if the value is not a string or if the string cannot
* be found in the array.
* This is a useful function for mapping strings to C enums.
*
* This function is similar to luaL_checkoption except that it accepts an
* array of std::string instead of char*, and returns a value of enumerated
* type E instead of int.
*
* \param l A Lua state.
* \param index Index of a string in the Lua stack.
* \param names An array of strings to search in. This array must be
* terminated by an empty string.
* \return The index (converted to the enumerated type E) where the string was
* found in the array.
*/
template<typename E>
E LuaContext::check_enum(lua_State* l, int index, const std::string names[]) 
{
  if(names[0].empty())
  {
      std::cerr << "Invalid list of names in check_enum\n";
  }
  const std::string& name = luaL_checkstring(l, index);
  for (int i = 0; !names[i].empty(); ++i) 
  {
    if (names[i] == name) 
	{
      return E(i);
    }
  }

   // The value was not found. Build an error message with possible values.
  std::string allowed_names;
  for (int i = 0; !names[i].empty(); ++i) 
  {
    allowed_names += "\"" + names[i] + "\", ";
  }
  allowed_names = allowed_names.substr(0, allowed_names.size() - 2);

  std::cerr << "Invalid name '" << name << "'. Allowed names are: " << allowed_names.c_str();
  throw; // Make sure the compiler is happy.
}

#endif