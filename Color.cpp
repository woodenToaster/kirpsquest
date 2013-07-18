/** @file Color.cpp */

#include "Color.h"

SDL_Surface * Color::format_surface = NULL;

Color Color::black;
Color Color::white;
Color Color::red;
Color Color::green;
Color Color::blue;
Color Color::yellow;
Color Color::magenta;
Color Color::cyan;

void Color::initialize() 
{
  format_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 1, 1, KQ_COLOR_DEPTH, 0, 0, 0, 0);

  black = Color( 0, 0, 0);
  white = Color(255, 255, 255);
  red = Color(255, 0, 0);
  green = Color( 0, 255, 0);
  blue = Color( 0, 0, 255);
  yellow = Color(255, 255, 0);
  magenta = Color(255, 0, 255);
  cyan = Color( 0, 255, 255);
}

/**
* @brief Uninitializes the color system.
*/
void Color::quit() 
{
  SDL_FreeSurface(format_surface);
  format_surface = NULL;
}

/**
* @brief Creates a default color with unspecified RGB values.
*/
Color::Color() 
{
  internal_color.r = 0;
  internal_color.g = 0;
  internal_color.b = 0;
  internal_value = 0;
}

/**
* @brief Copy constructor.
* @param other another color
*/
Color::Color(const Color &other): internal_value(other.internal_value), internal_color(other.internal_color) 
{

}

/**
* @brief Creates a color with the specified RGB values.
* @param r the red component (from 0 to 255)
* @param g the green component (from 0 to 255)
* @param b the blue component (from 0 to 255)
*/
Color::Color(int r, int g, int b) {
  internal_color.r = r;
  internal_color.g = g;
  internal_color.b = b;

  internal_value = SDL_MapRGB(format_surface->format, r, g, b);
}

/**
* @brief Creates a color from a 32-bit value.
*
* This constructor must be used only by low-level classes.
*
* @param internal_value The 32-bit value of the color to create.
*/
Color::Color(uint32_t internal_value):
  internal_value(internal_value) {

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  internal_color.r = (internal_value & 0x00ff0000) / 0x10000;
  internal_color.g = (internal_value & 0x0000ff00) / 0x100;
  internal_color.b = (internal_value & 0x000000ff);
#else
  internal_color.r = (internal_value & 0x000000ff);
  internal_color.g = (internal_value & 0x0000ff00) / 0x100;
  internal_color.b = (internal_value & 0x00ff0000) / 0x10000;
#endif
}

/**
* @brief Returns the 32-bit value representing this color.
*
* This function must be used only by low-level classes.
*
* @return the 32-bit value of this color
*/
uint32_t Color::get_internal_value() const 
{
  return internal_value;
}

/**
* @brief Returns the internal color encapsulated by this object.
*
* This function must be used only by low-level classes.
*
* @return the SDL color encapsulated
*/
SDL_Color* Color::get_internal_color() {
  return &internal_color;
}

/**
* @brief Returns the red, green and blue values of this color.
* @param r Red component to write.
* @param g Green component to write.
* @param b Blue component to write.
*/
void Color::get_components(int& r, int& g, int& b) const {

  r = internal_color.r;
  g = internal_color.g;
  b = internal_color.b;
}