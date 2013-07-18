/** @file StringResource.h */

#ifndef KQ_STRING_RESOURCE_H
#define KQ_STRING_RESOURCE_H

#include "Common.h"
#include <string>
#include <map>

/**
* \brief Provides access to strings translated in the current language.
*
* This class provides some strings loaded from an external file containing
* the texts in the current language.
* This class is used only to load simple strings such as the ones displayed
* in the menus.
* The messages displayed in the dialog box during the game come from another
* data file (see class DialogResource).
*/
class StringResource
{
private:

    static std::map<std::string, std::string> strings;

    // we don't need to instantiate this class
    StringResource();
    ~StringResource();

public:

    static void initialize();
    static void quit();

    static bool exists(const std::string& key);
    static const std::string& get_string(const std::string& key);
};
#endif