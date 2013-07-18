/** @file Sprite.cpp */

#include "Surface.h"
#include "Sprite.h"

std::map<std::string, SpriteAnimationSet*> Sprite::all_animation_sets;

/**
* \brief Initializes the sprites system.
*/
void Sprite::initialize()
{
}

/**
* \brief Uninitializes the sprites system.
*/
void Sprite::quit() 
{
  // delete the animations loaded
  std::map<std::string, SpriteAnimationSet*>::iterator it;
  for (it = all_animation_sets.begin(); it != all_animation_sets.end(); it++) 
  {
    delete it->second;
  }
  all_animation_sets.clear();
}

/**
* @brief Draws the sprite on a surface, with its current animation,
* direction and frame.
* @param dst_surface The destination surface.
* @param dst_position Coordinates on the destination surface
* (the origin will be placed at this position).
*/
void Sprite::raw_draw(Surface& dst_surface, const Rectangle& dst_position)
{

}