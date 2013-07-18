/** @file QuestResourceList.h */

#ifndef KQ_QUEST_RESOURCE_LIST_H
#define KQ_QUEST_RESOURCE_LIST_H

#include "Common.h"
#include <vector>
#include <map>
#include <string>

/**
* \brief This class reads the quest resource list from file project_db.dat
* and stores them.
*/
class QuestResourceList
{
public:
	/**
	 * \brief Types of existing resources in a quest.
	 */
	enum ResourceType
	{
		RESOURCE_MAP,
        RESOURCE_TILESET,
        RESOURCE_SPRITE,
        RESOURCE_MUSIC,
        RESOURCE_SOUND,
        RESOURCE_ITEM,
        RESOURCE_ENEMY,
        RESOURCE_LANGUAGE,
        RESOURCE_NB
	};

	static void initialize();
	static void quit();

	static const std::vector<std::string>& get_elements(ResourceType resource_type);

private:
	QuestResourceList();
	~QuestResourceList();
};
#endif