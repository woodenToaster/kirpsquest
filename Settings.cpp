/** @file Settings.cpp */

#include "Settings.h"
#include "FileTools.h"
#include "VideoManager.h"
#include "InputEvent.h"
#include "Sound.h"
#include "lua.hpp"
#include <sstream>

/**
* @brief Attempts to load the built-in settings from a file.
* @param file_name Settings file to read, relative to the quest write directory.
* @return true if settings were successfully loaded and applied.
*/
bool Settings::load(const std::string& file_name)
{
	const std::string& quest_write_dir = FileTools::get_quest_write_dir();
	if(quest_write_dir.empty())
	{
		std::cerr << "Error: no quest write directory specified in quest.dat\n";
	}
	const std::string& prefixed_file_name = quest_write_dir + "/" + file_name;
	std::cout << prefixed_file_name << '\n';
	if(!FileTools::data_file_exists(prefixed_file_name))
	{
		return false;
	}

	//Read the settings as a Lua data file

	return true;
}

/**
* \brief Needs Music::getvolume()
*  Attempts to save the built-in settings to a file.
* \param file_name Settings file to write, relative to the quest write directory.
* \return true if settings were successfully saved.
*/
bool Settings::save(const std::string& file_name)
{
	const std::string& quest_write_dir = FileTools::get_quest_write_dir();
	if(quest_write_dir.empty())
	{
		std::cerr << "Canot save settings: no quest write directory was specified in quest.dat\n";
	}

	const std::string& prefixed_file_name = quest_write_dir + "/" + file_name;

	std::ostringstream oss;
	VideoManager::VideoMode video_mode = VideoManager::get_instance()->get_video_mode();
	oss << "video_mode = \"" << VideoManager::video_mode_names[video_mode] << "\"\n";
    oss << "sound_volume = " << Sound::get_volume() << "\n";
	
    oss << "music_volume = " << 100 << "\n";
	if(!FileTools::get_language().empty())
	{
		oss << "language = \"" << FileTools::get_language() << "\"\n";
	}

	const std::string& text = oss.str();
	FileTools::data_file_save_buffer(prefixed_file_name, text.c_str(), text.size());
	return true;
}