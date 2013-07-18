/** @file Sound.h */

#ifndef KQ_SOUND_H
#define KQ_SOUND_H

#include "Common.h"
#include <string>
#include <list>
#include <map>
#include "al.h"
#include "alc.h"
#include "vorbis/vorbisfile.h"


//Finished
/**
* @brief Represents a sound effect that can be played in the program.
*
* This class also handles the initialization of the whole audio system.
* To create a sound, prefer the Sound::play() method
* rather than calling directly the constructor of Sound.
* This class is the only one that depends on the sound decoding library (libsndfile).
* This class and the Music class are the only ones that depend on the audio mixer library (OpenAL).
*/

class Sound
{
private:

	static ALCdevice* device;		
	static ALCcontext* context;		


	std::string id;									/**< id of this sound */
	ALuint buffer;									/**< the OpenAL buffer containing the PCM decoded data of this sound */
	std::list<ALuint> sources;						/**< the sources currently playing this sound */
	static std::list<Sound*> current_sounds;		/**< the sounds currently playing */
	static std::map<std::string, Sound> all_sounds;	/**< all sounds created before */

	static bool initialized;						/**< indicates that the audio system is initialized */
	static bool sounds_preloaded;					/**< true if load_all() was called */
	static float volume;							/**< the volume of sound effects (0.0 to 1.0) */

	ALuint decode_file(const std::string &file_name);
	bool update_playing();

public:

	//libvorbisfile

	/**
    * @brief Buffer containing an encoded sound file.
	*/
	struct SoundFromMemory
	{
		char* data;			/**< the buffer */
		size_t size;		/**< size of the buffer in bytes */
		size_t position;	/**< current position in the buffer */
		bool loop;			/**< true to restart the sound when finished */
	};

	// functions to load the encoded sound from memory
	 static ov_callbacks ogg_callbacks;           /**< vorbisfile object used to load the encoded sound from memory */
	 static size_t cb_read(void* ptr, size_t size, size_t nmemb, void* datasource);

	 Sound(const std::string& sound_id = "");
	 ~Sound();
	 void load();
	 bool start();

	 static void load_all();
	 static bool exists(const std::string& sound_id);
	 static void play(const std::string& sound_id);

	 static void initialize(int argc, char** argv);
     static void quit();
     static bool is_initialized();
     static void update();

     static int get_volume();
     static void set_volume(int volume);
};
#endif