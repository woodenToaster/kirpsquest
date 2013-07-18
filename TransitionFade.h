/** @file TransitionFade.h */

#ifndef KQ_TRANSITION_FADE_H
#define KQ_TRANSITION_FADE_H

#include "Common.h"
#include "Transition.h"

class TransitionFade: public Transition
{
private:

    bool finished;
    int alpha_start;
    int alpha_limit;
    int alpha_increment;
    int alpha; // current alpha value of the surface

    uint32_t next_frame_date;
    uint32_t delay;

    Surface* dst_surface;

public:

	// TODO:  pass the destination surface at creation time
    TransitionFade(Direction direction);
    ~TransitionFade();

    void set_delay(uint32_t delay);

    void start();
    bool is_started() const;
    bool is_finished() const;
    void notify_suspended(bool suspended);
    void update();
    void draw(Surface& surface);
};
#endif