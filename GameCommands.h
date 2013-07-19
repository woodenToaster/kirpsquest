/** @file GameCommands.h */

#ifndef KQ_GAME_COMMANDS_H
#define KQ_GAME_COMMANDS_H

#include "Common.h"
#include "InputEvent.h"
#inlcude <string>
#include <map>
#include <set>

/**
* \brief Stores the mapping between the in-game high-level commands
* and their keyboard and joypad bindings.
*
* This class receives the low-level keyboard and joypad events that occur
* during the game and then notifies the appropriate objects that a built-in
* game command was pressed or released.
* What we call a game command is a high-level notion such as "action" or
* "attack".
* The corresponding low-level input event can be a keyboard event or a
* joypad event.
*/
class GameCommands
{
public:
	/**
	 * \brief The built-in commands recognized by the engine during a game.
	 *
	 *  These high-level commands can be mapped onto the keyboard and the joypad.
	 */
	enum Command
	{
		NONE = -1,
		ACTION,
		ATTACK,
		ITEM_1,
		ITEM_2,
		PAUSE,
		RIGHT,
		UP,
		LEFT,
		DOWN,
		NB_COMMANDS
	};
	
	GameCommands(Game& game);
	~GameCommands();
	
	InputEvent::KeyboardKey get_keyboard_binding(Command command) const;
	void set_keyboard_binding(Command command, InputEvent::KeyboardKey keyboard_key);
    const std::string& get_joypad_binding(Command command) const;
    void set_joypad_binding(Command command, const std::string& joypad_string);

    void notify_input(InputEvent& event);
    bool is_command_pressed(Command command) const;
    int get_wanted_direction8() const;

    void customize(Command command, int callback_ref);
    bool is_customizing() const;
    Command get_command_to_customize() const;

    static bool is_joypad_string_valid(const std::string& joypad_string);
    static const std::string& get_command_name(Command command);
    static Command get_command_by_name(const std::string& command_name);

    static const std::string command_names[];

private:
	
	Savegame& get_savegame() const;
	
	//High-level resulting commands.
	void game_command_pressed(Command command);
	void game_command_released(Command command);
	
};
#endif