/** @file Transition.cpp */

#include "Transition.h"
#include "TransitionFade.h"
#include "System.h"
#include <iostream>

/**
* @brief Creates a transition effect.
* @param direction direction of the transition effect (in or out)
*/
Transition::Transition(Transition::Direction direction):
  game(NULL),
  direction(direction),
  previous_surface(NULL),
  suspended(false),
  when_suspended(0) 
{
}

Transition::~Transition()
{
}


/**
* @brief Returns the current game.
*
* Some transition effects need a game to run.
*
* @return The current game or NULL.
*/
Game* Transition::get_game() const 
{
  return game;
}

/**
* @brief Returns the direction of this transition effect.
* @returns the direction of this transition effect: Transition::IN or Transition::OUT
*/
Transition::Direction Transition::get_direction() const 
{
  return direction;
}

/**
* @brief Returns the surface to show during the OUT transition
* that was played before this IN transition.
* @return The previous surface or NULL.
*/
Surface* Transition::get_previous_surface() const 
{
  return previous_surface;
}

/**
* @brief Indicates the surface that was shown during the OUT transition
* that was played before this IN transition.
* @param previous_surface The previous surface or NULL.
*/
void Transition::set_previous_surface(Surface* previous_surface) 
{
  if(!(previous_surface == NULL || get_direction() != OUT))
  {
		 std::cerr << "Cannot show a previous surface with an OUT transition effect\n";
  }

  this->previous_surface = previous_surface;
}

/**
* @brief Returns whether this transition effect needs the previous surface.
* @return false
*/
bool Transition::needs_previous_surface() const 
{
  return false;
}

/**
* @brief Returns whether this transition is currently suspended.
* @return true if this transition is suspended.
*/
bool Transition::is_suspended() const 
{
  return suspended;
}

/**
* @brief Suspends or resumes this transition.
* @param suspended true to suspend it, false to resume it.
*/
void Transition::set_suspended(bool suspended) 
{
  if (suspended != this->suspended) 
  {
    this->suspended = suspended;
    if (suspended) 
	{
      when_suspended = System::now();
    }
    notify_suspended(suspended);
  }
}

/**
* @brief Returns the date when this transition was suspended if it is.
* @return The date when this transition was suspended or 0.
*/
uint32_t Transition::get_when_suspended() const 
{
  return when_suspended;
}