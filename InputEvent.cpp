/** @file InputEvent.cpp */

#include "InputEvent.h"

const InputEvent::KeyboardKey InputEvent::directional_keys[] =
{
	KEY_RIGHT, KEY_UP, KEY_LEFT, KEY_DOWN, KEY_NONE
};

std::map<InputEvent::KeyboardKey, std::string> InputEvent::keyboard_key_names;

/** @brief Implement mouse input events */

void InputEvent::initialize()
{
	//initialize keyboard
	SDL_EnableUNICODE(SDL_ENABLE);
	SDL_EnableKeyRepeat(0, 0);

	//initialize map of keyboard key names
	  keyboard_key_names[InputEvent::KEY_NONE] = "";
	  keyboard_key_names[InputEvent::KEY_BACKSPACE] = "backspace";
	  keyboard_key_names[InputEvent::KEY_TABULATION] = "tab";
	  keyboard_key_names[InputEvent::KEY_CLEAR] = "clear";
	  keyboard_key_names[InputEvent::KEY_RETURN] = "return";
	  keyboard_key_names[InputEvent::KEY_PAUSE] = "pause";
	  keyboard_key_names[InputEvent::KEY_ESCAPE] = "escape";
	  keyboard_key_names[InputEvent::KEY_SPACE] = "space";
	  keyboard_key_names[InputEvent::KEY_EXCLAMATION_MARK] = "!";
	  keyboard_key_names[InputEvent::KEY_DOULE_QUOTE] = "\"";
	  keyboard_key_names[InputEvent::KEY_HASH] = "#";
	  keyboard_key_names[InputEvent::KEY_DOLLAR] = "$";
	  keyboard_key_names[InputEvent::KEY_AMPERSAND] = "&";
	  keyboard_key_names[InputEvent::KEY_SINGLE_QUOTE] = "'";
	  keyboard_key_names[InputEvent::KEY_LEFT_PARENTHESIS] = "(";
	  keyboard_key_names[InputEvent::KEY_RIGHT_PARENTHESIS] = ")";
	  keyboard_key_names[InputEvent::KEY_ASTERISK] = "*";
	  keyboard_key_names[InputEvent::KEY_PLUS] = "+";
	  keyboard_key_names[InputEvent::KEY_COMMA] = ",";
	  keyboard_key_names[InputEvent::KEY_MINUS] = "-";
	  keyboard_key_names[InputEvent::KEY_PERIOD] = ".";
	  keyboard_key_names[InputEvent::KEY_SLASH] = "/";
	  keyboard_key_names[InputEvent::KEY_0] = "0";
	  keyboard_key_names[InputEvent::KEY_1] = "1";
	  keyboard_key_names[InputEvent::KEY_2] = "2";
	  keyboard_key_names[InputEvent::KEY_3] = "3";
	  keyboard_key_names[InputEvent::KEY_4] = "4";
	  keyboard_key_names[InputEvent::KEY_5] = "5";
	  keyboard_key_names[InputEvent::KEY_6] = "6";
	  keyboard_key_names[InputEvent::KEY_7] = "7";
	  keyboard_key_names[InputEvent::KEY_8] = "8";
	  keyboard_key_names[InputEvent::KEY_9] = "9";
	  keyboard_key_names[InputEvent::KEY_COLON] = ".";
	  keyboard_key_names[InputEvent::KEY_SEMICOLON] = ":";
	  keyboard_key_names[InputEvent::KEY_LESS] = "<";
	  keyboard_key_names[InputEvent::KEY_EQUALS] = "=";
	  keyboard_key_names[InputEvent::KEY_GREATER] = ">";
	  keyboard_key_names[InputEvent::KEY_QUESTION_MARK] = "?";
	  keyboard_key_names[InputEvent::KEY_AT] = "@";
	  keyboard_key_names[InputEvent::KEY_LEFT_BRACKET] = "[";
	  keyboard_key_names[InputEvent::KEY_BACKSLASH] = "\\";
	  keyboard_key_names[InputEvent::KEY_RIGHT_BRACKET] = "]";
	  keyboard_key_names[InputEvent::KEY_CARET] = "^";
	  keyboard_key_names[InputEvent::KEY_UNDERSCORE] = "_";
	  keyboard_key_names[InputEvent::KEY_BACKQUOTE] = "`";
	  keyboard_key_names[InputEvent::KEY_a] = "a";
	  keyboard_key_names[InputEvent::KEY_b] = "b";
	  keyboard_key_names[InputEvent::KEY_c] = "c";
	  keyboard_key_names[InputEvent::KEY_d] = "d";
	  keyboard_key_names[InputEvent::KEY_e] = "e";
	  keyboard_key_names[InputEvent::KEY_f] = "f";
	  keyboard_key_names[InputEvent::KEY_g] = "g";
	  keyboard_key_names[InputEvent::KEY_h] = "h";
	  keyboard_key_names[InputEvent::KEY_i] = "i";
	  keyboard_key_names[InputEvent::KEY_j] = "j";
	  keyboard_key_names[InputEvent::KEY_k] = "k";
	  keyboard_key_names[InputEvent::KEY_l] = "l";
	  keyboard_key_names[InputEvent::KEY_m] = "m";
	  keyboard_key_names[InputEvent::KEY_n] = "n";
	  keyboard_key_names[InputEvent::KEY_o] = "o";
	  keyboard_key_names[InputEvent::KEY_p] = "p";
	  keyboard_key_names[InputEvent::KEY_q] = "q";
	  keyboard_key_names[InputEvent::KEY_r] = "r";
	  keyboard_key_names[InputEvent::KEY_s] = "s";
	  keyboard_key_names[InputEvent::KEY_t] = "t";
	  keyboard_key_names[InputEvent::KEY_u] = "u";
	  keyboard_key_names[InputEvent::KEY_v] = "v";
	  keyboard_key_names[InputEvent::KEY_w] = "w";
	  keyboard_key_names[InputEvent::KEY_x] = "x";
	  keyboard_key_names[InputEvent::KEY_y] = "y";
	  keyboard_key_names[InputEvent::KEY_z] = "z";
	  keyboard_key_names[InputEvent::KEY_DELETE] = "delete";
	  keyboard_key_names[InputEvent::KEY_KP0] = "kp 0";
	  keyboard_key_names[InputEvent::KEY_KP1] = "kp 1";
	  keyboard_key_names[InputEvent::KEY_KP2] = "kp 2";
	  keyboard_key_names[InputEvent::KEY_KP3] = "kp 3";
	  keyboard_key_names[InputEvent::KEY_KP4] = "kp 4";
	  keyboard_key_names[InputEvent::KEY_KP5] = "kp 5";
	  keyboard_key_names[InputEvent::KEY_KP6] = "kp 6";
	  keyboard_key_names[InputEvent::KEY_KP7] = "kp 7";
	  keyboard_key_names[InputEvent::KEY_KP8] = "kp 8";
	  keyboard_key_names[InputEvent::KEY_KP9] = "kp 9";
	  keyboard_key_names[InputEvent::KEY_KP_PERIOD] = "kp .";
	  keyboard_key_names[InputEvent::KEY_KP_DIVIDE] = "kp /";
	  keyboard_key_names[InputEvent::KEY_KP_MULTIPLY] = "kp *";
	  keyboard_key_names[InputEvent::KEY_KP_MINUS] = "kp -";
	  keyboard_key_names[InputEvent::KEY_KP_PLUS] = "kp +";
	  keyboard_key_names[InputEvent::KEY_KP_ENTER] = "kp return";
	  keyboard_key_names[InputEvent::KEY_KP_EQUALS] = "kp =";
	  keyboard_key_names[InputEvent::KEY_UP] = "up";
	  keyboard_key_names[InputEvent::KEY_DOWN] = "down";
	  keyboard_key_names[InputEvent::KEY_RIGHT] = "right";
	  keyboard_key_names[InputEvent::KEY_LEFT] = "left";
	  keyboard_key_names[InputEvent::KEY_INSERT] = "insert";
	  keyboard_key_names[InputEvent::KEY_HOME] = "home";
	  keyboard_key_names[InputEvent::KEY_END] = "end";
	  keyboard_key_names[InputEvent::KEY_PAGE_UP] = "page up";
	  keyboard_key_names[InputEvent::KEY_PAGE_DOWN] = "page down";
	  keyboard_key_names[InputEvent::KEY_F1] = "f1";
	  keyboard_key_names[InputEvent::KEY_F2] = "f2";
	  keyboard_key_names[InputEvent::KEY_F3] = "f3";
	  keyboard_key_names[InputEvent::KEY_F4] = "f4";
	  keyboard_key_names[InputEvent::KEY_F5] = "f5";
	  keyboard_key_names[InputEvent::KEY_F6] = "f6";
	  keyboard_key_names[InputEvent::KEY_F7] = "f7";
	  keyboard_key_names[InputEvent::KEY_F8] = "f8";
	  keyboard_key_names[InputEvent::KEY_F9] = "f9";
	  keyboard_key_names[InputEvent::KEY_F10] = "f10";
	  keyboard_key_names[InputEvent::KEY_F11] = "f11";
	  keyboard_key_names[InputEvent::KEY_F12] = "f12";
	  keyboard_key_names[InputEvent::KEY_F13] = "f13";
	  keyboard_key_names[InputEvent::KEY_F14] = "f14";
	  keyboard_key_names[InputEvent::KEY_F15] = "f15";
	  keyboard_key_names[InputEvent::KEY_NUMLOCK] = "num lock";
	  keyboard_key_names[InputEvent::KEY_CAPSLOCK] = "caps lock";
	  keyboard_key_names[InputEvent::KEY_SCROLLOCK] = "scroll lock";
	  keyboard_key_names[InputEvent::KEY_RIGHT_SHIFT] = "right shift";
	  keyboard_key_names[InputEvent::KEY_LEFT_SHIFT] = "left shift";
	  keyboard_key_names[InputEvent::KEY_RIGHT_CONTROL] = "right control";
	  keyboard_key_names[InputEvent::KEY_LEFT_CONTROL] = "left control";
	  keyboard_key_names[InputEvent::KEY_RIGHT_ALT] = "right alt";
	  keyboard_key_names[InputEvent::KEY_LEFT_ALT] = "left alt";
	  keyboard_key_names[InputEvent::KEY_RIGHT_META] = "right meta";
	  keyboard_key_names[InputEvent::KEY_LEFT_META] = "left meta";
	  keyboard_key_names[InputEvent::KEY_LEFT_WINDOWS] = "right windows";
	  keyboard_key_names[InputEvent::KEY_RIGHT_WINDOWS] = "left windows";
}

/**
* \brief Quits the input event manager.
*/
void InputEvent::quit() 
{
  //Close SDL mouse
}
/** @brief Creates a keyboard event
 *  @param event the internal event to encapsulate */

InputEvent::InputEvent(const SDL_Event &event): internal_event(event)
{
}

InputEvent::~InputEvent()
{
}

/** @brief Return the first event from the event queue, or NULL if there is no event
 *
 *  @return the current event to handle, or NULL if there is no event */

InputEvent* InputEvent::get_event()
{
	InputEvent* result = NULL;
	SDL_Event internal_event;
	if(SDL_PollEvent(&internal_event))
	{
		result = new InputEvent(internal_event);
	}

	return result;
}

//global information

/** @brief Sets the keyboard repeat preferences.
 *  @param delay delay in milliseconds before the event begins repeating (0 means no repeating)
 *  @param interval interval in milliseconds between two events while repeating */

void InputEvent::set_key_repeat(int delay, int interval) 
{
  SDL_EnableKeyRepeat(delay, interval);
}

//event type

/** @brief Returns whether this event is a keyboard event.
 *  @return true if this is a keyboard event */

bool InputEvent::is_keyboard_event() 
{
  return internal_event.type == SDL_KEYDOWN || internal_event.type == SDL_KEYUP;
}

/** @brief Returns whether this event is a mouse event.
 *  @return true if this is a mouse event */

bool InputEvent::is_mouse_event()
{
	return internal_event.type == SDL_MOUSEMOTION || internal_event.type == SDL_MOUSEBUTTONDOWN
		|| internal_event.type == SDL_MOUSEBUTTONUP;
}

/** @brief Returns whether this event is a window event.
 *  @return true if this is a window event */

bool InputEvent::is_window_event() 
{
  return internal_event.type == SDL_QUIT; // other SDL window events are ignored
}

//keyboard

/** @brief Returns whether this event is a keyboard event corresponding to pressing a key.
 *
 *  @return true if this is a key pressed event */ 

bool InputEvent::is_keyboard_key_pressed() 
{
  return internal_event.type == SDL_KEYDOWN;
}

/**
* @brief Returns whether this event is a keyboard event
* corresponding to pressing one of the specified keys.
* @param keys an array of the keys to test, terminated by KEY_NONE
* @return true if this event corresponds to pressing one of those keys
*/
bool InputEvent::is_keyboard_key_pressed(const KeyboardKey* keys) 
{

  while (*keys != KEY_NONE) 
  {
    if (is_keyboard_key_pressed(keys)) 
	{
      return true;
    }
    keys++;
  }

  return false;
}

/**
* @brief Returns whether this event is a keyboard event
* corresponding to pressing one of the four directional keys.
* @return true if this event corresponds to pressing one of the four directional keys
*/
bool InputEvent::is_keyboard_direction_key_pressed() 
{
  return is_keyboard_key_pressed(directional_keys);
}

/**
* @brief Returns whether this event is a keyboard event
* corresponding to pressing a key other than the four directional keys.
* @return true if this event corresponds to pressing a key other than the four directional keys
*/
bool InputEvent::is_keyboard_non_direction_key_pressed() 
{
  return is_keyboard_key_pressed() && !is_keyboard_direction_key_pressed();
}

/**
* @brief Returns whether this event is a keyboard event
* corresponding to releasing a key.
* @return true if this is a key released event
*/
bool InputEvent::is_keyboard_key_released() 
{
  return internal_event.type == SDL_KEYUP;
}

/**
* @brief Returns whether this event is a keyboard event
* corresponding to releasing a specific key.
* @param key the key to test
* @return true if this event corresponds to releasing that key
*/
bool InputEvent::is_keyboard_key_released(KeyboardKey key) 
{
  return is_keyboard_key_released() && get_keyboard_key() == key;
}

/**
* @brief Returns whether this event is a keyboard event
* corresponding to releasing one of the specified keys.
* @param keys an array of the keys to test, terminated by KEY_NONE
* @return true if this event corresponds to releasing one of those keys
*/
bool InputEvent::is_keyboard_key_released(const KeyboardKey *keys) 
{
  while (*keys != KEY_NONE) 
  {
    if (is_keyboard_key_released(*keys)) 
	{
      return true;
    }
    keys++;
  }

  return false;
}

/**
* @brief Returns whether this event is a keyboard event
* corresponding to releasing one of the four directional keys.
* @return true if this event corresponds to releasing one of the four directional keys
*/
bool InputEvent::is_keyboard_direction_key_released() 
{
  return is_keyboard_key_released(directional_keys);
}

/**
* @brief Returns whether this event is a keyboard event
* corresponding to releasing a key other than the four directional keys.
* @return true if this event corresponds to releasing a key other than the four directional keys
*/
bool InputEvent::is_keyboard_non_direction_key_released() 
{
  return is_keyboard_key_released() && !is_keyboard_direction_key_released();
}

/**
* @brief Returns the key that was pressed or released during
* this keyboard event.
*
* If this is not a keyboard event, KEY_NONE is returned.
* The raw key is returned. If you want the corresponding character if any,
* see get_character().
*
* @return The key of this keyboard event.
*/
InputEvent::KeyboardKey InputEvent::get_keyboard_key() 
{
  if (!is_keyboard_event()) 
  {
    return KEY_NONE;
  }

  return KeyboardKey(internal_event.key.keysym.sym);
}

/**
* @brief Returns the Lua name of a keyboard key.
* @param key A keyboard key.
* @return The corresponding name (or an empty string for KEY_NONE).
*/
const std::string& InputEvent::get_keyboard_key_name(KeyboardKey key) 
{
  return keyboard_key_names[key];
}

/**
* @brief Returns a keyboard key given its name.
* @param keyboard_key_name The name of a keyboard key.
* @return The corresponding key, or KEY_NONE if this name is empty or unknown.
*/
InputEvent::KeyboardKey InputEvent::get_keyboard_key_by_name(const std::string& keyboard_key_name) {

  // TODO check that this traversal is not significant, otherwise make a reverse mapping.
  std::map<KeyboardKey, std::string>::iterator it;
  for (it = keyboard_key_names.begin(); it != keyboard_key_names.end(); it++) 
  {
    if (it->second == keyboard_key_name) 
	{
      return it->first;
    }
  }
  return KEY_NONE;
}

/**
* @brief Returns whether this event is a keyboard event
* corresponding to pressing a character key.
* @return true if this event corresponds to pressing a character key.
*/
bool InputEvent::is_character_pressed() 
{
  return internal_event.key.keysym.unicode != 0;
}

/**
* @brief Returns a UTF-8 representation of the character that was pressed during this keyboard event.
* @return The UTF-8 string corresponding to the key pressed, or an empty string if the key was not a character.
*/
const std::string InputEvent::get_character() {

  std::string result;
  if (is_character_pressed()) 
  {
    uint16_t utf16_char = (uint16_t) internal_event.key.keysym.unicode;
    char buffer[3] = { 0 };
    // SDL gives us UCS-2: convert it to UTF-8.
    if ((utf16_char & 0xFF80) != 0) 
	{
      // Two bytes.
      buffer[0] = (uint8_t) (0xC0 | utf16_char >> 6);
      buffer[1] = (uint8_t) (0x80 | (utf16_char & 0x3F));
    }
    else 
	{
      buffer[0] = (uint8_t) utf16_char;
    }
    result = buffer;
  }
  return result;
}

//TODO mouse

//common functions



//window event

/**
* @brief Returns whether this event corresponds to
* the user closing the window.
* @return true if this is a window closing event
*/
bool InputEvent::is_window_closing() 
{
  return internal_event.type == SDL_QUIT;
}

