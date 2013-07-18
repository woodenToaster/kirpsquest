/** @file TextSurface.h */

#ifndef KQ_TEXT_SURFACE_H
#define KQ_TEXT_SURFACE_H

#include "Common.h"
#include "Surface.h"
#include "Color.h"
#include "Rectangle.h"
#include "SDL_ttf.h"
#include "LuaContext.h"
#include <map>

struct lua_State;

class TextSurface: public Drawable
{
public:
	/**
	* Horizontal alignment of the text.
	*/
    enum HorizontalAlignment 
	{
      ALIGN_LEFT,		/**< the text is aligned to the left */
      ALIGN_CENTER,		/**< the text is centered */
      ALIGN_RIGHT		/**< the text is aligned to the right */
    };

    /**
	* Vertical alignment of the text.
	*
	*/
    enum VerticalAlignment 
	{
      ALIGN_TOP,		/**< the text is aligned to the top */
      ALIGN_MIDDLE,		/**< the text is aligned in the middle */
      ALIGN_BOTTOM		/**< the text is aligned to the bottom */
    };

    /**
	* Rendering mode of the text.
	*/
    enum RenderingMode 
	{
      TEXT_SOLID,			/**< letters are drawn without effect (faster, visible pixels) */
      TEXT_ANTIALIASING		/**< letters are drawn with a smooth effect (slower) */
    };


private:
	//stores the data of a font
	struct FontData
	{
		std::string file_name;
		int font_size;
		char* buffer;
		SDL_RWops* rw;
		TTF_Font* internal_font;
		Surface* bitmap;
	};

	static std::map<std::string, FontData> fonts;
	static std::string default_font_id;

	std::string font_id;
	HorizontalAlignment horizontal_alignment;
	VerticalAlignment vertical_alignment;
	RenderingMode rendering_mode;
	Color text_color;

	int x;
	int y;
	Surface* surface;
	Rectangle text_position;

	std::string text;

	void rebuild();
	void rebuild_bitmap();
	void rebuild_ttf();

public:
	static void initialize();
    static void quit();
    static int l_font(lua_State* l);

    TextSurface(int x, int y);
    TextSurface(int x, int y, HorizontalAlignment horizontal_alignment, VerticalAlignment vertical_alignment);
    ~TextSurface();

    static bool has_font(const std::string& font_id);
    const std::string& get_font();
    void set_font(const std::string& font_id);
    HorizontalAlignment get_horizontal_alignment();
    void set_horizontal_alignment(HorizontalAlignment horizontal_alignment);
    VerticalAlignment get_vertical_alignment();
    void set_vertical_alignment(VerticalAlignment vertical_alignment);
    void set_alignment(HorizontalAlignment horizontal_alignment, VerticalAlignment vertical_alignment);
    RenderingMode get_rendering_mode();
    void set_rendering_mode(RenderingMode rendering_mode);
    const Color& get_text_color();
    void set_text_color(const Color& color);
    void set_text_color(int r, int g, int b);

    void set_position(int x, int y);
    int get_x();
    void set_x(int x);
    int get_y();
    void set_y(int y);
    const std::string& get_text();
    void set_text(const std::string& text);
    bool is_empty();
    void add_char(char c);

    int get_width();
    int get_height();
    const Rectangle get_size();

    void raw_draw(Surface& dst_surface, const Rectangle& dst_position);
    void raw_draw_region(const Rectangle& region, Surface& dst_surface, const Rectangle& dst_position);
    void draw_transition(Transition& transition);

    const std::string& get_lua_type_name() const;
};
#endif
