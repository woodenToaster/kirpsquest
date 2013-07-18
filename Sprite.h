/** @file Sprite.h */

#ifndef KQ_SPRITE_H
#define KQ_SPRITE_H

#include "Common.h"
#include "Drawable.h"
#include <map>

/**
* @brief Represents an animated sprite.
*
* A sprite is represented as a set of animations
* (i.e. an instance of SpriteAnimationSet),
* a current animation,
* a current direction and a current frame.
* Several sprites can have the same animation set (i.e. they share
* the same SpriteAnimationSet object).
*
* A sprite can be drawn directly on a surface, or it can
* be attached to a map entity.
*/

class Sprite: public Drawable
{
public:
	static void initialize();
	static void quit();

	Sprite(const std::string& id);
	~Sprite();

	//update and draw
	void update();
	void raw_draw(Surface& dst_surfaace, const Rectangle& dst_position);
	
private:
	LuaContext* lua_context;           /**< The Solarus Lua API (NULL means no callbacks for this sprite). 
									    *   TODO move this to ExportableToLua */

	// animation set
    static std::map<std::string, SpriteAnimationSet*> all_animation_sets;
    const std::string animation_set_id;			/**< id of this sprite's animation set */
    SpriteAnimationSet& animation_set;			/**< animation set of this sprite */

};
#endif