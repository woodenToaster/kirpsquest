/** @file Music.cpp */

#include "Music.h"
#include "FileTools.h"

const int Music::nb_buffers;
float Music::volume = 1.0;
Music* Music::current_music = NULL;
std::map<std::string, Music> Music::all_musics;

const std::string Music::none = "none";
const std::string Music::unchanged = "same";

/**
* \brief Creates a new music.
* \param music_id id of the music (file name without extension)
*/
Music::Music(const std::string& music_id): id(music_id), format(OGG), initialized(false)
{
	if(!is_initialized() || music_id == none)
	{
		return;
	}
	
	for(int i = 0; i < nb_buffers; i++)
	{
		buffers[i] = AL_NONE;
	}
	source = AL_NONE;
}

/**
* \brief Destroys the music.
*/
Music::~Music() 
{
  if (!is_initialized()) 
  {
    return;
  }

  if(current_music == this) 
  {
    stop();
  }
}

/**
* \brief Initializes the music system.
*/
void Music::initialize() 
{
  initialized = true;
  set_volume(100);
}

/**
* \brief Exits the music system.
*/
void Music::quit() 
{
  if (is_initialized()) 
  {
    all_musics.clear();
  }
}

/**
* \brief Returns whether the music system is initialized.
* \return true if the music system is initilialized
*/
bool Music::is_initialized() 
{
  return initialized;
}

/**
* \brief Returns the current volume of music.
* \return the volume (0 to 100)
*/
int Music::get_volume() 
{
  return (int) (volume * 100.0 + 0.5);
}

/**
* \brief Sets the volume of musics.
* \param volume the new volume (0 to 100)
*/
void Music::set_volume(int volume) 
{
  volume = std::min(100, std::max(0, volume));
  Music::volume = volume / 100.0;

  if (current_music != NULL) 
  {
    alSourcef(current_music->source, AL_GAIN, Music::volume);
  }
}

/**
* \brief Returns the music currently playing.
* \return the current music, or NULL if no music is being played
*/
Music* Music::get_current_music() 
{
  return current_music;
}

/**
* \brief Returns the id of the music currently playing.
* \return the id of the current music, or "none" if no music is being played
*/
const std::string& Music::get_current_music_id() 
{
  return current_music != NULL ? current_music->id : none;
}


















