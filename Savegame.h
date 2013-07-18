/** @file Savegame.h */

#ifndef KQ_SAVE_GAME_H
#define KQ_SAVE_GAME_H

#include "Common.h"
#include "ExportableToLua.h"
#include <map>

struct lua_State;

/**
* \brief Manages the game data saved.
*
* This class provides read and write access to the saved data.
*/
class Savegame: public ExportableToLua
{
public:

	Savegame(MainLoop& main_loop, const std::string& file_name);
	~Savegame();

	//file state
	bool is_empty();
	void save();
	const std::string& get_file_name();

	//data
	bool is_string(const std::string& key);
	const std::string& get_string(const std::string& key);
    void set_string(const std::string& key, const std::string& value);
    bool is_integer(const std::string& key);
    int get_integer(const std::string& key);
    void set_integer(const std::string& key, int value);
    bool is_boolean(const std::string& key);
    bool get_boolean(const std::string& key);
    void set_boolean(const std::string& key, bool value);
    void unset(const std::string& key);

	// unsaved data
    MainLoop& get_main_loop();
    LuaContext& get_lua_context();
    //Equipment& get_equipment();
    Game* get_game();
    void set_game(Game* game);
    void notify_game_started();
    void notify_game_finished();

	virtual const std::string& get_lua_type_name() const;

private:

	struct SavedValue
	{
		enum
		{
			VALUE_STRING,
			VALUE_INTEGER,
			VALUE_BOOLEAN
		} type;

		std::string string_data;
		int int_data; //Also used for boolean
	};

	std::map<std::string, SavedValue> saved_values;

	bool empty;
	std::string file_name;			/**< Savegame file name relative to the quest write directory. */
	std::string prefixed_file_name;	/**< Savegame file name relative to the engine write directory. */
	MainLoop& main_loop;
	//Equipment equipment;
	Game* game;						/**< NULL if this savegame is not currently running */

	void load();
    static int l_newindex(lua_State* l);

    void set_initial_values();
    void set_default_keyboard_controls();
    void set_default_joypad_controls();
};
#endif