/** @file TextSurfaceAPI.cpp */

#include "lua.hpp"
#include "LuaContext.h"
#include "TextSurface.h"
#include "FileTools.h"
#include "StringResource.h"

const std::string LuaContext::text_surface_module_name = "kq.text_surface";

static const std::string rendering_mode_names[] = {
    "solid",
    "antialiasing",
    "" // Sentinel.
};

static const std::string horizontal_alignment_names[] = {
    "left",
    "center",
    "right",
    "" // Sentinel.
};

static const std::string vertical_alignment_names[] = {
    "top",
    "middle",
    "bottom",
    "" // Sentinel.
};

/**
* \brief Initializes the text surface features provided to Lua.
*/
void LuaContext::register_text_surface_module() 
{
	static const luaL_Reg methods[] = 
	{
		{ "create", text_surface_api_create },
		/*{ "get_horizontal_alignment", text_surface_api_get_horizontal_alignment },
		{ "set_horizontal_alignment", text_surface_api_set_horizontal_alignment },
		{ "get_vertical_alignment", text_surface_api_get_vertical_alignment },
		{ "set_vertical_alignment", text_surface_api_set_vertical_alignment },
		{ "get_font", text_surface_api_get_font },
		{ "set_font", text_surface_api_set_font },
		{ "get_rendering_mode", text_surface_api_get_rendering_mode },
		{ "set_rendering_mode", text_surface_api_set_rendering_mode },
		{ "get_color", text_surface_api_get_color },
		{ "set_color", text_surface_api_set_color },
		{ "get_text", text_surface_api_get_text },
		{ "set_text", text_surface_api_set_text },
		{ "set_text_key", text_surface_api_set_text_key },
		{ "get_size", text_surface_api_get_size },
		{ "draw", drawable_api_draw },
		{ "draw_region", drawable_api_draw_region },
		{ "fade_in", drawable_api_fade_in },
		{ "fade_out", drawable_api_fade_out },
		{ "get_xy", drawable_api_get_xy },
		{ "set_xy", drawable_api_set_xy },
		{ "get_movement", drawable_api_get_movement },
		{ "stop_movement", drawable_api_stop_movement },*/
		{ NULL, NULL }
    };

	static const luaL_Reg metamethods[] = 
	{
        { "__gc", drawable_meta_gc },
        { NULL, NULL }
    };
    register_type(text_surface_module_name, methods, metamethods);
}

/**
* \brief Pushes a text surface userdata onto the stack.
* \param l a Lua context
* \param text_surface a text surface
*/
void LuaContext::push_text_surface(lua_State* l, TextSurface& text_surface) 
{
  push_userdata(l, text_surface);
}

/**
* \brief Implementation of kq.text_surface.create().
* \param l The Lua context that is calling this function.
* \return Number of values to return to Lua.
*/
int LuaContext::text_surface_api_create(lua_State* l)
{
	TextSurface* text_surface = new TextSurface(0,0);

	if(lua_gettop(l) > 0)
	{
		// Traverse the table, looking for properties.
		lua_pushnil(l); //First key
		while(lua_next(l, 1) != 0)
		{
			const std::string& key = luaL_checkstring(l, 2);
			if (key == "font") 
			{
				const std::string& font_id = luaL_checkstring(l, 3);
				text_surface->set_font(font_id);
			}
			else if (key == "rendering_mode")
			{
				TextSurface::RenderingMode mode = check_enum<TextSurface::RenderingMode>(l, 3, rendering_mode_names);
				text_surface->set_rendering_mode(mode);
			}
			else if (key == "horizontal_alignment") 
			{
				TextSurface::HorizontalAlignment alignment = check_enum<TextSurface::HorizontalAlignment>(l, 3, horizontal_alignment_names);
				text_surface->set_horizontal_alignment(alignment);
			}
			else if (key == "vertical_alignment") 
			{
				TextSurface::VerticalAlignment alignment =
				check_enum<TextSurface::VerticalAlignment>(l, 3, vertical_alignment_names);
				text_surface->set_vertical_alignment(alignment);
			}
			else if (key == "color") 
			{
				Color color = check_color(l, 3);
				text_surface->set_text_color(color);
			}
			else if (key == "text") 
			{
				const std::string& text = luaL_checkstring(l, 3);
				text_surface->set_text(text);
			}
			else if (key == "text_key") 
			{
				const std::string& text_key = luaL_checkstring(l, 3);

				if (!StringResource::exists(text_key)) 
				{
					delete text_surface;
					std::cerr << "No value with key '" << text_key << "' in strings.dat\n";
				}
				text_surface->set_text(StringResource::get_string(text_key));
			}
			else 
			{
				delete text_surface;
				std::cerr << "Invalid key '" << key	<< "' for text surface properties\n";
			}
			lua_pop(l, 1); // Pop the value, leave the key for the next iteration.
		}

	}
	get_lua_context(l).add_drawable(text_surface);

	push_text_surface(l, *text_surface);
	return 1;
}
