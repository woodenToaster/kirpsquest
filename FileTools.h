/** @file FileTools.h */

#ifndef KQ_FILETOOLS_H
#define KQ_FILETOOLS_H

#include <string>
#include <map>
#include <iostream>
#include <cstdint>

/** @brief Finished */

struct lua_State;

class FileTools
{
public:
	static void initialize(int argc, char* argv[]);
	static void quit();

	static bool data_file_exists(const std::string& file_name);
	static std::istream& data_file_open(const std::string& file_name, bool language_specific);
	static void data_file_open_buffer(const std::string& file_name, char** buffer, size_t* size);
	static void data_file_close(const std::istream& data_file);
	static void data_file_close_buffer(char* buffer);
	static void data_file_save_buffer(const std::string& file_name, const char* buffer, size_t size);
	static void data_file_delete(const std::string& file_name);

	static void read(std::istream& is, int& value);
    static void read(std::istream& is, uint32_t& value);
    static void read(std::istream& is, std::string& value);

	//Writing files
	static void set_kq_write_dir(const std::string& kq_write_dir);
	static void set_quest_write_dir(const std::string& quest_write_dir);
	static const std::string& get_quest_write_dir();

	//Languages
	static bool has_language(const std::string& language_code);
    static void set_language(const std::string& language_code);
    static const std::string& get_language();
    static const std::string& get_default_language();
    static const std::map<std::string, std::string>& get_languages();


private:
	static void initialize_languages();
	static std::string get_base_write_dir();
	static int l_language(lua_State* l);

	static std::string kq_write_dir;
	static std::string quest_write_dir;
	

	static std::map<std::string, std::string> languages;
	static std::string language_code;
	static std::string default_language_code;
};
#endif