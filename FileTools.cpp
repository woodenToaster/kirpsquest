/** @file FileTools.cpp */

#include "FileTools.h"
#include "LuaContext.h"
#include "StringResource.h"
#include "physfs.h"
#include <iostream>
#include <sstream>

std::string FileTools::kq_write_dir;
std::string FileTools::quest_write_dir;
std::string FileTools::language_code;
std::string FileTools::default_language_code;
std::map<std::string, std::string> FileTools::languages;

void FileTools::initialize(int argc, char* argv[])
{
	PHYSFS_init(argv[0]);

	std::string quest_path = "kq";

	/*if(argc < 1 && argv[argc - 1][0] != '-')
	{
		quest_path = argv[argc - 1];
	}*/

	std::cout << "Opening quest '" << quest_path << "'" << std::endl;

	std::string dir_quest_path = quest_path + "/data";
	std::string archive_quest_path = quest_path + "/data.kq";

	const std::string& base_dir = PHYSFS_getBaseDir();
	PHYSFS_addToSearchPath(base_dir.c_str(), 1);
	PHYSFS_addToSearchPath(dir_quest_path.c_str(), 1);
	PHYSFS_addToSearchPath(archive_quest_path.c_str(), 1);
	PHYSFS_addToSearchPath((base_dir + dir_quest_path).c_str(), 1);
	PHYSFS_addToSearchPath((base_dir + archive_quest_path).c_str(), 1);

	set_kq_write_dir(KQ_WRITE_DIR);
	
	//initialize_languages();
}

/**
* \brief Quits the file tools.
*/
void FileTools::quit() 
{
  //DialogResource::quit();
  StringResource::quit();
  PHYSFS_deinit();
}

/**
* @brief Returns whether a file exists in the quest data directory or
* in Solarus write directory.
* @param file_name a file name relative to the quest data directory
* or to Solarus write directory.
* @return true if this file exists.
*/
bool FileTools::data_file_exists(const std::string& file_name)
{
	return PHYSFS_exists(file_name.c_str());
}

void FileTools::initialize_languages()
{
	static const std::string file_name = "languages/languages.dat";
	
	//Read the language file
	lua_State* l = luaL_newstate();
	size_t size;
	char* buffer;
	FileTools::data_file_open_buffer(file_name, &buffer, &size);
	std::cout << size << "\n";

	/*
	for(int i = 0; i < size; i++)
	{
		std::cout << buffer[i];
	}*/

	luaL_loadbuffer(l, buffer, size, file_name.c_str());
	data_file_close_buffer(buffer);

	lua_register(l, "language", l_language);
	lua_pcall(l, 0, 0, 0);
	lua_pop(l, 1);
	lua_close(l);
}

/**
* @brief Opens in reading a text file in the Solarus data directory.
*
* The file name is relative to the Solarus data directory.
* The program is stopped with an error message if the file cannot be open.
* Don't forget to close the stream with data_file_close().
*
* @param file_name name of the file to open
* @param language_specific true if the file is specific to the current language
* @return the input stream
*/
std::istream& FileTools::data_file_open(const std::string& file_name, bool language_specific) 
{
  size_t size;
  char* buffer;
  data_file_open_buffer(file_name, &buffer, &size);

  // create an input stream
  std::istringstream* is = new std::istringstream(std::string(buffer, size));
  data_file_close_buffer(buffer);
  return *is;
}

/**
* @brief Closes a text file previously open with data_file_open().
* @param data_file the input stream to close
*/
void FileTools::data_file_close(const std::istream& data_file) 
{
  delete &data_file;
}

void FileTools::data_file_open_buffer(const std::string& file_name, char** buffer, size_t* size)
{
	std::cerr << file_name.c_str() << '\n';
	if(!PHYSFS_exists(file_name.c_str()))
	{
		std::cerr << "File does not exist.\n";
	}
	PHYSFS_file* file = PHYSFS_openRead(file_name.c_str());
	if(file == NULL)
	{
		std::cerr << "Error opening file\n";
		std::cerr << PHYSFS_getLastError() << '\n';
	}
	
	*size = PHYSFS_fileLength(file);

	*buffer = new char[*size];
	if(buffer == NULL)
	{
		std::cerr << "Error allocating memory\n";
	}

	PHYSFS_read(file, *buffer, 1, PHYSFS_uint32(*size));
	PHYSFS_close(file);
}

/**
* @brief Reads an integer value from an input stream.
*
* 
*
* @param is an input stream
* @param value the value read
*/
void FileTools::read(std::istream& is, int& value) 
{
  if (!(is >> value)) 
  {
    std::cerr << "Cannot read integer from input stream\n";
  }
}

/**
* @brief Reads an integer value from an input stream.
*
* 
*
* @param is an input stream
* @param value the value read
*/
void FileTools::read(std::istream& is, uint32_t& value) 
{
  int v;
  read(is, v);
  if(v < 0)
  {
	  std::cerr << "Positive integer value expected from input stream\n";
  }
  value = (uint32_t) v;
}

/**
* @brief Reads a string value from an input stream.
*
* 
*
* @param is an input stream
* @param value the value read
*/
void FileTools::read(std::istream& is, std::string& value) 
{
  if (!(is >> value)) 
  {
    std::cerr << "Cannot read string from input stream\n";
  }
}

int FileTools::l_language(lua_State* l)
{
	luaL_checktype(l, 1, LUA_TTABLE);

	std::string code = LuaContext::check_string_field(l, 1, "code");
    std::string name = LuaContext::check_string_field(l, 1, "name");
	
	if(languages.empty())
	{
		default_language_code = code;
	}
	languages[code] = name;

	return 0;
}

/**
* \brief Returns the current language.
*
* The language-specific data are be loaded from the directory of this language.
*
* \return code of the language, or an empty string if no language is set
*/
const std::string& FileTools::get_language() 
{
  return language_code;
}

void FileTools::set_kq_write_dir(const std::string& kq_write_dir)
{
	FileTools::kq_write_dir = kq_write_dir; //.kirpsquest

	if(!PHYSFS_setWriteDir(get_base_write_dir().c_str())) //D:\users\Chris
	{
		std::cout << "Error: Cannot write in user directory\n";
	}

	PHYSFS_mkdir(kq_write_dir.c_str());

	const std::string& full_write_dir = get_base_write_dir() + "\\" + kq_write_dir;
	if(!PHYSFS_setWriteDir(full_write_dir.c_str())) //D:\users\Chris\.kirpsquest
	{
		std::cout << "Cannot set write directory to " << full_write_dir << '\n';
	}

	if(!quest_write_dir.empty())
	{
		set_quest_write_dir(quest_write_dir);
	}
}

/**
* \brief Saves a buffer into a data file.
* \param file_name Name of the file to write, relative to Solarus write directory.
* \param buffer The buffer to save.
* \param size Number of bytes to write.
*
*/
void FileTools::data_file_save_buffer(const std::string& file_name, const char* buffer, size_t size)
{
	//Open the file to write
	PHYSFS_file* file = PHYSFS_openWrite(file_name.c_str());
	if(file == NULL)
	{
		std::cerr << "Cannot open file '" << file_name << "' for writing: " << PHYSFS_getLastError();
	}

	//Save the memory buffer
	if(PHYSFS_write(file, buffer, PHYSFS_uint32(size), 1) == -1)
	{
		std::cerr << "Cannot write file '" << file_name << "': " << PHYSFS_getLastError();
	}
	PHYSFS_close(file);
}

/**
* @brief Closes a data buffer previously open with data_file_open_buffer().
* @param buffer the buffer to close
*/
void FileTools::data_file_close_buffer(char* buffer) 
{
  delete[] buffer;
}

std::string FileTools::get_base_write_dir()
{
	return std::string(PHYSFS_getUserDir());
}

void FileTools::set_quest_write_dir(const std::string& quest_write_dir)
{
	FileTools::quest_write_dir = quest_write_dir;

	if(!quest_write_dir.empty())
	{
		std::cout << "Quest write directory is: " << quest_write_dir << '\n';
		PHYSFS_mkdir(quest_write_dir.c_str());
	}
}

const std::string& FileTools::get_quest_write_dir()
{
	return quest_write_dir;
}