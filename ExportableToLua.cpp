/** @file ExportableToLua.cpp */

#include "ExportableToLua.h"

ExportableToLua::ExportableToLua(): refcount(0) 
{
}

ExportableToLua::~ExportableToLua() 
{
  //Debug::check_assertion(refcount == 0, StringConcat()
     // << "This object is still used somewhere else: refcount is " << refcount);
}
/**
* @brief Returns the current refcount of this object.
*
* You must not delete this object if its refcount is not zero.
*
* @return the current refcount of this object
*/
int ExportableToLua::get_refcount() const 
{
  return refcount;
}

/**
* @brief Increments the refcount of this object.
*/
void ExportableToLua::increment_refcount()
{
  refcount++;
}

/**
* @brief Decrements the refcount of this object.
*/
void ExportableToLua::decrement_refcount() 
{
  //Debug::check_assertion(refcount > 0, "The refcount is already 0");
  refcount--;
}

