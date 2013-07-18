/** @file QuestProperties.h */

#ifndef KQ_QUEST_PROPERTIES_H
#define KQ_QUEST_PROPERTIES_H

#include "Common.h"
#include <string>

struct lua_State;

/**
* @brief Finished
* This class reads the quest properties from file quest.lua
* and applies them.
*
* These properties are general information about the quest. They include:
* - the writing directory for savegames of this quest,
* - the window title bar.
*
* The main reason why these quest properties are stored as a data file
* (even if they are also available through the Solarus scripting API)
* is to make them easily editable in quest editors.
*/
class QuestProperties
{
private:
	static int l_quest(lua_State* l);
	MainLoop& main_loop;

public:
	QuestProperties(MainLoop& main_loop);
	~QuestProperties();
	void load();
};
#endif