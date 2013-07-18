/** @file QuestProperties.cpp */

#include "QuestProperties.h"
#include "FileTools.h"
#include "lua.hpp"
#include "LuaContext.h"
#include "VideoManager.h"

QuestProperties::QuestProperties(MainLoop& main_loop): main_loop(main_loop)
{
}

QuestProperties::~QuestProperties()
{
}

void QuestProperties::load()
{
	const std::string& file_name = "quest.dat";
	lua_State* l = luaL_newstate();
	size_t size;
	char* buffer;
	FileTools::data_file_open_buffer(file_name, &buffer, &size);
	luaL_loadbuffer(l, buffer, size, file_name.c_str());
	FileTools::data_file_close_buffer(buffer);

	lua_register(l, "quest", l_quest);
	if(lua_pcall(l, 0, 0, 0) != 0)
	{
		std::cerr << "Error: failed to load quest.dat\n";
		lua_pop(l, 1);
	}
	lua_close(l);
}

int QuestProperties::l_quest(lua_State* l)
{
	luaL_checktype(l, 1, LUA_TTABLE);
	const std::string& quest_write_dir = LuaContext::opt_string_field(l, 1, "write_dir", "");
	const std::string& title_bar = LuaContext::opt_string_field(l, 1, "title_bar", "");

	FileTools::set_quest_write_dir(quest_write_dir);
	if(!title_bar.empty())
	{
		VideoManager::get_instance()->set_window_title(title_bar);
	}
	return 0;
}