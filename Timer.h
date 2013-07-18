/** @file Timer.h */

#ifndef KQ_TIMER_H
#define KQ_TIMER_H

#include "Common.h"
#include "ExportableToLua.h"

/**@brief Finished */

class Timer: public ExportableToLua
{
public:
	Timer(uint32_t duration);
	~Timer();

	bool is_with_sound();
	void set_with_sound(bool with_sound);
	bool is_suspended();
	void set_suspended(bool suspended);
	bool is_suspended_with_map();
	void set_suspended_with_map(bool suspended_with_map);
	bool is_finished();

	void update();
	void notify_map_suspended(bool suspended);

	virtual const std::string& get_lua_type_name() const;

private:
	//timer
	uint32_t expiration_date;		/**< date when the timer is finished */
	bool finished;					/**< indicates that the timer is finished */

	bool suspended_with_map;		/**< whether the timer should be suspended when the map is */
	bool suspended;					/**< indicates whether the timer is suspended */
	uint32_t when_suspended;		/**< date when the timer was suspended */

	//sound
	uint32_t next_sound_date;		/**< date when the next clock sound effect is played */
};
#endif