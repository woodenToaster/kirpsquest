/** @file ExportableToLua.h */

#ifndef KQ_EXPORTABLE_TO_LUA_H
#define KQ_EXPORTABLE_TO_LUA_H

#include "Common.h"
#include <string>

/**
 * @brief Interface of a C++ type that can also exist as a Lua userdata.
 *
 * An optional reference counting system is provided.
 * It allows you to share safely the object between C++ and Lua,
 * no matter which world stops using it first.
 *
 * Lua bindings use the this system for all userdata created by Lua.
 * When the refcount gets to zero, it means that there is no more Lua userdata
 * for this object, and that C++ is not using the object either.
 * Therefore, you know you can delete the object.
 *
 * Note that if you don't need the refcount system, you don't have to use it.
 * You can just leave the refcount to zero and delete the object normally.
 * You don't need to care about refcounts if your object is not allocated
 * dynamically or if you don't share pointers.
 * If you export your object to Lua, refcounts will be very helpful.
 */
class ExportableToLua 
{

  public:

    ExportableToLua();
    virtual ~ExportableToLua();

    // Reference counting.
    int get_refcount() const;
    void increment_refcount();
    void decrement_refcount();

    /**
     * @brief Returns the name identifying this type in Lua.
     * @return the name identifying this type in Lua
     */
    virtual const std::string& get_lua_type_name() const = 0;

  private:

    int refcount;                /**< number of pointers to the object
                                  * including the Lua ones
                                  * (0 means that it can be deleted) */
};

#endif

