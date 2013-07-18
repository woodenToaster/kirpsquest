/** @file Settings.h */

#ifndef KQ_SETTINGS_H
#define KQ_SETTINGS_H

#include "Common.h"
#include <string>

/**
* @brief Loads and saves the built-in settings of the quest.
*
* The settings include the language, the video mode and the audio volume.
*/
class Settings
{
public:
	static bool load(const std::string& file_name);
	static bool save(const std::string& file_name);
};
#endif