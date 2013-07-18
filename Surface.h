/** @file Surface.h */

#ifndef KQ_SURFACE_H
#define KQ_SURFACE_H

#include "Common.h"
#include "Drawable.h"
#include "Rectangle.h"
#include "SDL.h"
#include "Color.h"

/** @brief Finished  */

class Surface: public Drawable
{
	// low-level classes allowed to directly manipulate the internal SDL surface encapsulated
	friend class TextSurface;
	friend class VideoManager;
	friend class PixelBits;

private:
	SDL_Surface* internal_surface;				 /**< the SDL_Surface encapsulated */
	bool internal_surface_created;				 /**< indicates that internal_surface was allocated from this class */

	uint32_t get_pixel32(int idx_pixel);
	SDL_Surface* get_internal_surface();
    uint32_t get_mapped_pixel(int idx_pixel, SDL_PixelFormat* dst_format);

protected:
	//virtual functions from Drawable
	void raw_draw(Surface& dst_surface, const Rectangle& dst_position);
	void raw_draw_region(const Rectangle& region, Surface& dst_surface, const Rectangle& dst_position);
	void draw_transition(Transition& transition);

public:
	/**
	* @brief The base directory to use when opening image files.
	*/
    enum ImageDirectory 
	{
      DIR_DATA,			/**< the root directory of the data package */
      DIR_SPRITES,		/**< the sprites subdirectory of the data package (default) */
	  DIR_LANGUAGE		/**< the language-specific image directory of the data package for the current language. */
    };

    Surface(int width = KQ_SCREEN_WIDTH, int height = KQ_SCREEN_HEIGHT);
    Surface(const Rectangle& size);
    Surface(const std::string& file_name, ImageDirectory base_directory = DIR_SPRITES);
    Surface(SDL_Surface* internal_surface);
    Surface(const Surface& other);
    ~Surface();

    int get_width() const;
    int get_height() const;
    const Rectangle get_size() const;

    Color get_transparency_color();
    void set_transparency_color(const Color& color);
    void set_opacity(int opacity);
    void set_clipping_rectangle(const Rectangle& clipping_rectangle = Rectangle());
    void fill_with_color(Color& color);
    void fill_with_color(Color& color, const Rectangle& where);

    void draw_region(const Rectangle& src_position, Surface& dst_surface);
    void draw_region(const Rectangle& src_position, Surface& dst_surface, const Rectangle& dst_position);

    const std::string& get_lua_type_name() const;
};

#endif
