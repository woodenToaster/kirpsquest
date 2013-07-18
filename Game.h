/** @file Game.h */

#ifndef KQ_GAME_H
#define KQ_GAME_H

#include "Common.h"
#include "Transition.h"

class MainLoop;

/** @brief Needs Savegame, Hero, Map, DialogBox, KeysEffects, LuaContext, Command 
 *
 *  The game shows the current map and handles all game elements.
 */

class Game
{
private:
	//main objects
	MainLoop& main_loop;			/**< the main loop object */
	//Savegame* savegame			/**< the saved game data */
	//Hero* hero;

	//current game state
	bool paused;
	bool pause_key_available;
	bool started;					/**< true if this game is running, false if it is not yet started or being closed */
	bool restarting;				/**< true if the game will be restarted */

	//controls
	//GameCommands* commands		/**< this object receives the keyboard and mouse events */
	//KeysEffect* keys_effect;		/**< current effect associated with the main game keys */

	//map
	//Map* current_map;
	//Map* next_map;				/**< if not NULL, the hero is changing from current_map to next_map
	
	//graphics
	//DialogBox dialog_box

	//update functions
	void update_keys_effect();
    void update_dialog_box();
    void update_gameover_sequence();
    void notify_map_changed();

public:
	//creation and destruction
    //Game(MainLoop& main_loop, Savegame* savegame);
    ~Game();

	void start();
	void stop();

	//global objects
	MainLoop& get_main_loop();
	LuaContext& get_lua_context();
	//Hero& get_hero();
    const Rectangle& get_hero_xy();
    //GameCommands& get_commands();
    //KeysEffect& get_keys_effect();
    //Savegame& get_savegame();
    //Equipment& get_equipment();
				
	// functions called by the main loop
    bool notify_input(InputEvent &event);
    void update();
    void draw(Surface& dst_surface);

	 // game controls
    //void notify_command_pressed(GameCommands::Command command);
    //void notify_command_released(GameCommands::Command command);

    // map
    bool has_current_map();
    //Map& get_current_map();
    void set_current_map(const std::string& map_id, const std::string& destination_name);

	// current game state
    bool is_paused();
    bool is_dialog_enabled();
    bool is_showing_gameover();
    bool is_suspended(); // true if at least one of the three functions above returns true

	//pause
	bool can_pause();
    bool can_unpause();
    bool is_pause_key_available();
    void set_pause_key_available(bool pause_key_available);
    void set_paused(bool paused);

	 // dialog box
    //DialogBox& get_dialog_box();

    // game over
    void start_gameover_sequence();
    void get_back_from_death();
    void restart();

};

#endif