/** @file InputEvent.h */

#ifndef KQ_INPUT_EVENT_H
#define KQ_INPUT_EVENT_H

#include "Common.h"
#include "SDL.h"
#include <string>
#include <map>

/** @brief Make sure all mouse functionality is implemented
 *
 *  This low-level class provides an API for input events (keyboards, mouse and window events).
 *  It encapsulates the library-dependent events. */

class InputEvent
{
public:
	enum KeyboardKey 
	{
      KEY_NONE = 0,
      KEY_BACKSPACE = 8,
      KEY_TABULATION = 9,
      KEY_CLEAR = 12,
      KEY_RETURN = 13,
      KEY_PAUSE = 19,
      KEY_ESCAPE = 27,
      KEY_SPACE = 32,
      KEY_EXCLAMATION_MARK = 33,
      KEY_DOULE_QUOTE = 34,
      KEY_HASH = 35,
      KEY_DOLLAR = 36,
      KEY_AMPERSAND = 38,
      KEY_SINGLE_QUOTE = 39,
      KEY_LEFT_PARENTHESIS = 40,
      KEY_RIGHT_PARENTHESIS = 41,
      KEY_ASTERISK = 42,
      KEY_PLUS = 43,
      KEY_COMMA = 44,
      KEY_MINUS = 45,
      KEY_PERIOD = 46,
      KEY_SLASH = 47,
      KEY_0 = 48,
      KEY_1 = 49,
      KEY_2 = 50,
      KEY_3 = 51,
      KEY_4 = 52,
      KEY_5 = 53,
      KEY_6 = 54,
      KEY_7 = 55,
      KEY_8 = 56,
      KEY_9 = 57,
      KEY_COLON = 58,
      KEY_SEMICOLON = 59,
      KEY_LESS = 60,
      KEY_EQUALS = 61,
      KEY_GREATER = 62,
      KEY_QUESTION_MARK = 63,
      KEY_AT = 64,
      KEY_LEFT_BRACKET = 91,
      KEY_BACKSLASH = 92,
      KEY_RIGHT_BRACKET = 93,
      KEY_CARET = 94,
      KEY_UNDERSCORE = 95,
      KEY_BACKQUOTE = 96,
      KEY_a = 97,
      KEY_b = 98,
      KEY_c = 99,
      KEY_d = 100,
      KEY_e = 101,
      KEY_f = 102,
      KEY_g = 103,
      KEY_h = 104,
      KEY_i = 105,
      KEY_j = 106,
      KEY_k = 107,
      KEY_l = 108,
      KEY_m = 109,
      KEY_n = 110,
      KEY_o = 111,
      KEY_p = 112,
      KEY_q = 113,
      KEY_r = 114,
      KEY_s = 115,
      KEY_t = 116,
      KEY_u = 117,
      KEY_v = 118,
      KEY_w = 119,
      KEY_x = 120,
      KEY_y = 121,
      KEY_z = 122,
      KEY_DELETE = 127,

      KEY_KP0 = 256,
      KEY_KP1 = 257,
      KEY_KP2 = 258,
      KEY_KP3 = 259,
      KEY_KP4 = 260,
      KEY_KP5 = 261,
      KEY_KP6 = 262,
      KEY_KP7 = 263,
      KEY_KP8 = 264,
      KEY_KP9 = 265,
      KEY_KP_PERIOD = 266,
      KEY_KP_DIVIDE = 267,
      KEY_KP_MULTIPLY = 268,
      KEY_KP_MINUS = 269,
      KEY_KP_PLUS = 270,
      KEY_KP_ENTER = 271,
      KEY_KP_EQUALS = 272,

      KEY_UP = 273,
      KEY_DOWN = 274,
      KEY_RIGHT = 275,
      KEY_LEFT = 276,
      KEY_INSERT = 277,
      KEY_HOME = 278,
      KEY_END = 279,
      KEY_PAGE_UP = 280,
      KEY_PAGE_DOWN = 281,

      KEY_F1 = 282,
      KEY_F2 = 283,
      KEY_F3 = 284,
      KEY_F4 = 285,
      KEY_F5 = 286,
      KEY_F6 = 287,
      KEY_F7 = 288,
      KEY_F8 = 289,
      KEY_F9 = 290,
      KEY_F10 = 291,
      KEY_F11 = 292,
      KEY_F12 = 293,
      KEY_F13 = 294,
      KEY_F14 = 295,
      KEY_F15 = 296,

      KEY_NUMLOCK = 300,
      KEY_CAPSLOCK = 301,
      KEY_SCROLLOCK = 302,
      KEY_RIGHT_SHIFT = 303,
      KEY_LEFT_SHIFT = 304,
      KEY_RIGHT_CONTROL = 305,
      KEY_LEFT_CONTROL = 306,
      KEY_RIGHT_ALT = 307,
      KEY_LEFT_ALT = 308,
      KEY_RIGHT_META = 309,
      KEY_LEFT_META = 310,
      KEY_LEFT_WINDOWS = 311,
      KEY_RIGHT_WINDOWS = 312
    };

private:
	static const KeyboardKey directional_keys[];					/**< array of the keyboard directional keys */
	SDL_Event internal_event;										/**< the internal event encapsulated */
	static std::map<KeyboardKey, std::string> keyboard_key_names;	/**< names of all existing keyboard keys */

public:
	static void initialize();
	static void quit();

private:
	InputEvent(const SDL_Event &event);

public:
	~InputEvent();

	//retrieve the current event
	static InputEvent* get_event();

	//global information
	static void set_key_repeat(int delay, int interval);
	
	//event type
	bool is_keyboard_event();
	bool is_window_event();
	bool is_mouse_event();

	//keyboard
	bool is_keyboard_key_pressed();
	bool is_keyboard_key_pressed(KeyboardKey key);
	bool is_keyboard_key_pressed(const KeyboardKey* keys);
	bool is_keyboard_direction_key_pressed();
	bool is_keyboard_non_direction_key_pressed();

	bool is_keyboard_key_released();
    bool is_keyboard_key_released(KeyboardKey key);
    bool is_keyboard_key_released(const KeyboardKey* keys);
    bool is_keyboard_direction_key_released();
    bool is_keyboard_non_direction_key_released();

	KeyboardKey get_keyboard_key();
	static const std::string& get_keyboard_key_name(InputEvent::KeyboardKey key);
    static InputEvent::KeyboardKey get_keyboard_key_by_name(const std::string& keyboard_key_name);

	bool is_character_pressed();
	const std::string get_character();

	//mouse
	bool is_mouse_Lbutton_pressed();
	bool is_mouse_Lbutton_released();
	bool is_mouse_Rbutton_pressed();
	bool is_mouse_Rbutton_released();
	bool is_mouse_moved();

	//common
	int get_direction();
	bool is_pressed();
	bool is_direction_pressed();
	bool is_non_direction_pressed();
	bool is_released();

	//window event
	bool is_window_closing();
};

#endif