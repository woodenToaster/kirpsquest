/** @file QuestResourceList.cpp */

#include "QuestResourceList.h"
#include "FileTools.h"
#include "LuaContext.h"
#include <sstream>

namespace
{
	const std::string resource_type_names[] =
	{
		"map",
		"tileset",
		"sprite",
		"music",
		"sound",
		"item",
		"enemy",
		"language",
		"", // Sentinel for Lua.
	};

	std::vector<std::string> resource_elements[QuestResourceList::RESOURCE_NB];

	int l_resource_element(lua_State* l)
	{
		QuestResourceList::ResourceType resource_type = 
			LuaContext::check_enum<QuestResourceList::ResourceType>(l, 1, resource_type_names);
		const std::string& id = LuaContext::check_string_field(l, 2, "id");

		resource_elements[resource_type].push_back(id);

		return 0;
	}
}

/**
* \brief Reads the quest resource list file data file quest.lua and stores
* the list of resources.
*/
void QuestResourceList::initialize()
{
	//Read the quest resource list file
	const std::string& file_name = "project_db.dat";
	lua_State* l = luaL_newstate();
	size_t size;
	char* buffer;
	FileTools::data_file_open_buffer(file_name, &buffer, &size);
	luaL_loadbuffer(l, buffer, size, file_name.c_str());
	FileTools::data_file_close_buffer(buffer);

	//We register only one C function for all resource types.
	lua_register(l, "resource", l_resource_element);
	for(int i = 0; i < RESOURCE_NB; ++i)
	{
		std::ostringstream oss;
		oss << "function" << resource_type_names[i] << "(t) resource('" << resource_type_names[i] << "', t) end";
		luaL_dostring(l, oss.str().c_str());
	}

	if(lua_pcall(l, 0, 0, 0) != 0)
	{
		std::cerr << "Failed to load quest resource list 'project_db.dat': " << lua_tostring(l, -1);
		lua_pop(l, 1);
	}
	lua_close(l);
}

/**
* \brief Destroys what was created by initialize().
*/
void QuestResourceList::quit() 
{
  for (int i = 0; i < RESOURCE_NB; ++i) 
  {
    resource_elements[i].clear();
  }
}

/**
* \brief Returns the list of element IDs of the specified resource type.
* \param resource_type A type of resource.
* \return The IDs of all declared element of this type.
*/
const std::vector<std::string>& QuestResourceList::get_elements(ResourceType resource_type) 
{
  if(!(resource_type >= 0 && resource_type < RESOURCE_NB))
  {
	  std::cerr << "Invalid resource type\n";
  }
  return resource_elements[resource_type];
}