/** @file Rectangle.cpp */

#include "Rectangle.h"
#include <iostream>

/**
* @brief Creates a rectangle, specifying its properties.
* @param x x coordinate of the top-left corner
* @param y y coordinate of the top-left corner
* @param width the rectangle's width
* @param height the rectangle's height
*/
Rectangle::Rectangle(int x, int y, int width, int height) 
{
  rect.x = x;
  rect.y = y;
  rect.w = width;
  rect.h = height;
}

/**
* @brief Copy constructor.
* @param other the rectangle to copy
*/
Rectangle::Rectangle(const Rectangle &other): rect(other.rect) 
{
}

/**
* @brief Destructor.
*/
Rectangle::~Rectangle() 
{
}

/**
* @brief Returns whether the specified point is inside this rectangle.
* @param x x coordinate of the point
* @param y y coordinate of the point
* @return true if the point is in this rectangle
*/
bool Rectangle::contains(int x, int y) const 
{
  return x >= get_x() && x < get_x() + get_width() && y >= get_y() && y < get_y() + get_height();
}

/**
* @brief Returns whether the specified rectangle is inside this rectangle.
* @param other another rectangle
* @return true if the specified rectangle is inside this rectangle
*/
bool Rectangle::contains(const Rectangle &other) const 
{
  return contains(other.get_x(), other.get_y())
    && contains(other.get_x() + other.get_width() - 1, other.get_y() + other.get_height() - 1);
}

/**
* @brief Returns whether or not another rectangle overlaps this one.
* @param other another rectangle
* @return true if the two rectangles overlap
*/
bool Rectangle::overlaps(const Rectangle &other) const 
{
  int x1 = get_x();
  int x2 = x1 + get_width();
  int x3 = other.get_x();
  int x4 = x3 + other.get_width();

  bool overlap_x = (x3 < x2 && x1 < x4);

  int y1 = get_y();
  int y2 = y1 + get_height();
  int y3 = other.get_y();
  int y4 = y3 + other.get_height();

  bool overlap_y = (y3 < y2 && y1 < y4);

  return overlap_x && overlap_y;
}

/**
* @brief Returns the center point of this rectangle
* @return the center point (represented as a zero-sized rectangle)
*/
Rectangle Rectangle::get_center() 
{
  return Rectangle(get_x() + get_width() / 2, get_y() + get_height() / 2);
}

/**
* @brief Prints a rectangle to an output stream.
* @param stream the stream
* @param rectangle a rectangle
*/
std::ostream & operator <<(std::ostream &stream, const Rectangle &rectangle) 
{
  stream << "(" << rectangle.get_x() << "," << rectangle.get_y() << ") x ("
    << rectangle.get_width() << "," << rectangle.get_height() << ")";
  return stream;
}

/**
* @brief Returns the SDL rectangle encapsulated by this object.
*
* This function must be used only by other low-level classes (typically Surface)
* as it is library dependent.
*
* @return the internal rectangle encapsulated
*/
SDL_Rect * Rectangle::get_internal_rect() {

  return &rect;
}



