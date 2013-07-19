/** @file Music.h"

#ifndef KQ_MUSIC_H
#define KQ_MUSIC_H

#include "Common.h"
#include "Sound.h"

/**
* \brief Represents a music that can be played.
*
* A music should be in format .ogg. 
* Only one music can be played at a time.
* Before using this class, the audio system should have been
* initialized, by calling Sound::initialize().
* Sound and Music are the only classes that depends on audio libraries.
*/
class Music
{
public:
	static const std::string none;		/**< special id indicating that there is no music */
	static const std::string unchanged; /**< special id indicating that the music is the same as before */
	
	Music(const std::string& music_id = none);
	~Music();
	
	static void initialize();
	static void quit();
	static bool is_initialized();
	static void update();
	
	static int get_volume();
	static void set_volume(int volume);
	
	static void find_music_file(const std::string& music_id, std::string& file_name);
	static bool exists(const std::string& music_id);
	static void play(const std::string& music_id);
	static Music* get_current_music();
	static const std::string& get_current_music_id();
	
	bool start();
	void stop();
	bool is_paused();
	void set_paused(bool pause);
	
	void decode_ogg(ALuint destination_buffer, ALsizei nb_samples);
	
private:
	
	std::string id; 											/**< id of this music */
    std::string file_name; 										/**< name of the file to play */
    
    // OGG specific
    OggVorbis_File ogg_file; 									/**< the file used by the vorbisfile lib */
    Sound::SoundFromMemory ogg_mem; 							/**< the encoded music loaded in memory, 
																 *passed to the vorbisfile lib as user data */

    static const int nb_buffers = 8;
    ALuint buffers[nb_buffers]; 								/**< multiple buffers used to stream the music */
    ALuint source; 												/**< the OpenAL source streaming the buffers */

    static float volume; 										/**< volume of musics (0.0 to 1.0) */
	bool initialized;
    static Music* current_music; 								/**< the music currently played (if any) */
    static std::map<std::string, Music> all_musics; 			/**< all musics created before */

    void update_playing();
};
#endif