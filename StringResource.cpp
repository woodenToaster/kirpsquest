/** @file StringResource.cpp */

#include "StringResource.h"
#include "FileTools.h"

std::map<std::string, std::string> StringResource::strings;

/**
* \brief Constructor.
*/
StringResource::StringResource() 
{
}

/**
* \brief Destructor.
*/
StringResource::~StringResource() 
{
}

/**
* \brief Initializes the text resource by loading all strings.
*
* The strings are loaded from the language-specific file "text/strings.dat"
* and stored into memory for future access by get_string().
*/
void StringResource::initialize()
{
	strings.clear();
	std::istream& file = FileTools::data_file_open("text/strings.dat", false);
	std::string line;

	//Read each line
	int i = 0;
	while(std::getline(file, line))
	{
		i++;

		//Ignore empty lines or lines starting with '#'
		size_t index = line.find_first_of(" \t");
		
		//Insert Debug assertion

		std::string key = line.substr(0, index);

		//get the value
		do
		{
			index++;
		} while (index < line.size() && (line[index] == ' ' || line[index] == '\t' || line[index] == '\r'));

		//Insert Debug assertion

		std::string value = line.substr(index);

		if(value[value.size() - 1] == '\r')
		{
			//If the file has DOS line endings, remove the trailing '\r'
			value = value.substr(0, value.size() - 1);
		}
		strings[key] = value;
	}

	FileTools::data_file_close(file);
}

/**
* \brief Closes the text resource.
*/
void StringResource::quit() 
{
  strings.clear();
}

/**
* \brief Returns whether a string exists in the language-specific file
* "text/strings.dat" for the current language.
* \param key Id of a string.
* \return true if the string exists.
*/
bool StringResource::exists(const std::string& key) 
{
  return strings.find(key) != strings.end();
}

/**
* \brief Returns a string stored in the language-specific file
* "text/strings.dat" for the current language.
* \param key Id of the string to retrieve. It must exist.
* \return The corresponding localized string.
*/
const std::string& StringResource::get_string(const std::string& key) 
{
	if(!exists(key))
	{
		std::cerr << "Cannot find string w3ith ky '" << key << "'\n";
	}
    return strings[key];
}