/** @file Sound.cpp */

#include <cstring>
#include <cmath>
#include <sstream>
#include <vector>
#include "Sound.h"
#include "FileTools.h"

ALCdevice* Sound::device = NULL;
ALCcontext* Sound::context = NULL;
bool Sound::initialized = false;
bool Sound::sounds_preloaded = false;
float Sound::volume = 1.0;
std::list<Sound*> Sound::current_sounds;
std::map<std::string, Sound> Sound::all_sounds;
ov_callbacks Sound::ogg_callbacks = 
{
	cb_read,
	NULL,
	NULL,
	NULL
};

/**
* @brief Creates a new Ogg Vorbis sound.
* @param sound_id id of the sound: name of a .ogg file in the sounds subdirectory,
* without the extension (.ogg is added automatically)
*/
Sound::Sound(const std::string& sound_id): id(sound_id), buffer(AL_NONE) 
{
}

/**
* @brief Destroys the sound.
*/
Sound::~Sound() 
{
  if (is_initialized())
  {
    // stop the sources where this buffer is attached
    std::list<ALuint>::iterator it;
    for (it = sources.begin(); it != sources.end(); it++) 
	{
		ALuint source = (*it);
		alSourceStop(source);
		alSourcei(source, AL_BUFFER, 0);
		alDeleteSources(1, &source);
    }
    alDeleteBuffers(1, &buffer);
    current_sounds.remove(this);
  }
}

/**
* @brief Initializes the audio (music and sound) system.
*
* This method should be called when the application starts.
* If the argument -no-audio is provided, this function has no effect and
* there will be no sound.
*
* @param argc command-line arguments number
* @param argv command-line arguments
*/
void Sound::initialize(int argc, char** argv)
{
	//check the no-audio option
	//implement later

	//initialize OpenAL
	device = alcOpenDevice(NULL);
	if(!device)
	{
		std::cout << "Error:  could not open audio device\n";
	}
	ALCint attr[] = {ALC_FREQUENCY, 32000, 0};
	context = alcCreateContext(device, attr);
	if(!context)
	{
		std::cout << "Error:  could not create audio context\n";
		alcCloseDevice(device);
		return;
	}
	if (!alcMakeContextCurrent(context)) 
	{
		std::cout << "Cannot activate audio context\n";
		alcDestroyContext(context);
		alcCloseDevice(device);
		return;
    }

	alGenBuffers(0, AL_NONE);  // Necessary on some systems to avoid errors with the first sound loaded

	initialized = true;
    set_volume(100);

	//initialize the music system
	//Music::initialize();
}

/**
* @brief Closes the audio (music and sound) system.
*
* This method should be called when exiting the application.
*/
void Sound::quit() 
{
  if (is_initialized()) 
  {
    // uninitialize the music subsystem
    //Music::quit();

    // clear the sounds
    all_sounds.clear();

    // uninitialize OpenAL

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    context = NULL;
    alcCloseDevice(device);
    device = NULL;

    initialized = false;
  }
}

/**
* @brief Returns whether the audio (music and sound) system is initialized.
* @return true if the audio (music and sound) system is initilialized
*/
bool Sound::is_initialized() 
{
  return initialized;
}

/**
* @brief Loads and decodes all sounds listed in the game database.
*/
void Sound::load_all() 
{
	
  if (is_initialized() && !sounds_preloaded) 
  {
    // open the resource database file
    static const std::string file_name = "project_db.dat";
    std::istream& database_file = FileTools::data_file_open(file_name, false);
    std::string line;

    while (std::getline(database_file, line)) 
	{
      if (line.size() == 0 || line[0] == '\r') 
	  {
        continue;
      }

      int resource_type;
      std::string resource_id, resource_name;
      std::istringstream iss(line);
      FileTools::read(iss, resource_type);
      FileTools::read(iss, resource_id);
      FileTools::read(iss, resource_name);

      if (resource_type == 4) 
	  { 
		// it's a sound
        if (all_sounds.count(resource_id) == 0) 
		{
          all_sounds[resource_id] = Sound(resource_id);
          all_sounds[resource_id].load();
        }
      }
    }
    FileTools::data_file_close(database_file);
 
    sounds_preloaded = true;
  }
}

/**
* \brief Returns the current volume of sound effects.
* \return the volume (0 to 100)
*/
int Sound::get_volume() 
{
  return (int) (volume * 100.0 + 0.5);
}

/**
* @brief Sets the volume of sound effects.
* @param volume the new volume (0 to 100)
*/
void Sound::set_volume(int volume) 
{
  volume = std::min(100, std::max(0, volume));
  Sound::volume = volume / 100.0;
}

/**
* @brief Updates the audio (music and sound) system.
*
* This function is called repeatedly by the game.
*/
void Sound::update() 
{
  // update the playing sounds
  Sound* sound;
  std::list<Sound*> sounds_to_remove;
  std::list<Sound*>::iterator it;
  for (it = current_sounds.begin(); it != current_sounds.end(); it++) 
  {
    sound = *it;
    if (!sound->update_playing()) 
	{
      sounds_to_remove.push_back(sound);
    }
  }

  for (it = sounds_to_remove.begin(); it != sounds_to_remove.end(); it++) 
  {
    sound = *it;
    current_sounds.remove(sound);
  }

  // also update the music
  //Music::update();
}

/**
* @brief Updates this sound when it is playing.
* @return true if the sound is still playing, false if it is finished.
*/
bool Sound::update_playing() 
{
  // see whether a source playing this sound has finished playing
  ALuint source = *sources.begin();
  ALint status;
  alGetSourcei(source, AL_SOURCE_STATE, &status);

  if (status != AL_PLAYING) 
  {
    sources.pop_front();
    alSourcei(source, AL_BUFFER, 0);
    alDeleteSources(1, &source);
  }

  return sources.size() != 0;
}

/**
* @brief Returns whether a sound exists.
* @param sound_id id of the sound to test
* @return true if the sound exists
*/
bool Sound::exists(const std::string& sound_id) 
{
  std::ostringstream oss;
  oss << "sounds/" << sound_id << ".ogg";
  return FileTools::data_file_exists(oss.str());
}

/**
* @brief Starts playing the specified sound.
* @param sound_id id of the sound to play
*/
void Sound::play(const std::string& sound_id) 
{
  if (all_sounds.count(sound_id) == 0) 
  {
    all_sounds[sound_id] = Sound(sound_id);
  }

  all_sounds[sound_id].start();
}

/**
* @brief Loads and decodes the sound into memory.
*/
void Sound::load() 
{
  std::string file_name = (std::string) "sounds/" + id;
  if (id.find(".") == std::string::npos) 
  {
	file_name += ".ogg";
  }

  // Create an OpenAL buffer with the sound decoded by the library.
  buffer = decode_file(file_name);

  // buffer is now AL_NONE if there was an error.
}

/**
* @brief Plays the sound.
* @return true if the sound was loaded successfully, false otherwise
*/
bool Sound::start() 
{
  bool success = false;

  if (is_initialized()) 
  {
    if (buffer == AL_NONE) 
	{ 
	  // first time: load and decode the file
      load();
    }

    if (buffer != AL_NONE) 
	{
      // create a source
      ALuint source;
      alGenSources(1, &source);
      alSourcei(source, AL_BUFFER, buffer);
      alSourcef(source, AL_GAIN, volume);

      // play the sound
      int error = alGetError();
      if (error != AL_NO_ERROR) 
	  {
        std::cerr << "Cannot attach buffer " << buffer << " to the source to play sound '" << id << "': error " << error;
        alDeleteSources(1, &source);
      }
      else 
	  {
        sources.push_back(source);
        current_sounds.remove(this); // to avoid duplicates
        current_sounds.push_back(this);
        alSourcePlay(source);
        error = alGetError();
        if (error != AL_NO_ERROR) 
		{
          std::cerr << "Cannot play sound '" << id << "': error " << error;
        }
        else 
		{
          success = true;
        }
      }
    }
  }
  return success;
}

/**
* @brief Loads the specified sound file and decodes its content into an OpenAL buffer.
* @param file_name name of the file to open
* @return the buffer created, or AL_NONE if the sound could not be loaded
*/
ALuint Sound::decode_file(const std::string& file_name) 
{
  ALuint buffer = AL_NONE;

  if (!FileTools::data_file_exists(file_name)) 
  {
    std::cerr << "Cannot find sound file '" << file_name << "'";
    return AL_NONE;
  }

  // load the sound file
  SoundFromMemory mem;
  mem.loop = false;
  mem.position = 0;
  FileTools::data_file_open_buffer(file_name, &mem.data, &mem.size);

  OggVorbis_File file;
  int error = ov_open_callbacks(&mem, &file, NULL, 0, ogg_callbacks);

  if (error) 
  {
    std::cerr << "Cannot load sound file '" << file_name << "' from memory: error " << error;
  }
  else 
  {
    // read the encoded sound properties
    vorbis_info* info = ov_info(&file, -1);
    ALsizei sample_rate = ALsizei(info->rate);

    ALenum format = AL_NONE;
    if (info->channels == 1) 
	{
      format = AL_FORMAT_MONO16;
    }
    else if (info->channels == 2) 
	{
      format = AL_FORMAT_STEREO16;
    }

    if (format == AL_NONE) 
	{
      std::cerr << "Invalid audio format for sound file '" << file_name << "'";
    }
    else 
	{
      // decode the sound with vorbisfile
      std::vector<char> samples;
      int bitstream;
      long bytes_read;
      long total_bytes_read = 0;
      char samples_buffer[4096];
      do 
	  {
        bytes_read = ov_read(&file, samples_buffer, 4096, 0, 2, 1, &bitstream);
        if (bytes_read < 0) 
		{
          std::cerr << "Error while decoding ogg chunk in sound file '" << file_name << "': " << bytes_read;
        }
        else 
		{
          total_bytes_read += bytes_read;
          if (format == AL_FORMAT_STEREO16) 
		  {
            samples.insert(samples.end(), samples_buffer, samples_buffer + bytes_read);
          }
          else 
		  {
            // mono sound files make no sound on some machines
            // workaround: convert them on-the-fly into stereo sounds
            // TODO find a better solution
            for (int i = 0; i < bytes_read; i += 2) 
			{
              samples.insert(samples.end(), samples_buffer + i, samples_buffer + i + 2);
              samples.insert(samples.end(), samples_buffer + i, samples_buffer + i + 2);
            }
            total_bytes_read += bytes_read;
          }
        }
      }
      while (bytes_read > 0);

      // copy the samples into an OpenAL buffer
      alGenBuffers(1, &buffer);
      alBufferData(buffer, AL_FORMAT_STEREO16, (ALshort*) &samples[0], ALsizei(total_bytes_read), sample_rate);
      if (alGetError() != AL_NO_ERROR) 
	  {
        std::cerr << "Cannot copy the sound samples of '" << file_name << " into buffer " << buffer << std::endl;
        buffer = AL_NONE;
      }
    }
    ov_clear(&file);
  }

  FileTools::data_file_close_buffer(mem.data);

  return buffer;
}

/**
* @brief Loads an encoded sound from memory.
*
* This function respects the prototype specified by libvorbisfile.
*
* @param ptr pointer to a buffer to load
* @param size size
* @param nb_bytes number of bytes to load
* @param datasource source of the data to read
* @return number of bytes loaded
*/
size_t Sound::cb_read(void* ptr, size_t size, size_t nb_bytes, void* datasource) 
{
  SoundFromMemory* mem = (SoundFromMemory*) datasource;

  if (mem->position >= mem->size) 
  {
    if (mem->loop)
	{
      mem->position = 0;
    }
    else
	{
      return 0;
    }
  }
  else if (mem->position + nb_bytes >= mem->size) 
  {
    nb_bytes = mem->size - mem->position;
  }

  memcpy(ptr, mem->data + mem->position, nb_bytes);
  mem->position += nb_bytes;

  return nb_bytes;
}

