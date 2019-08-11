#pragma once

#include <string>
#include "LuaCallCpp.h"
#include <windows.h>
#include <lua.hpp>
#include <iostream>
#include <map>

class IOStuff {

	lua_State *state;
	std::string dirpath;
	std::map<std::string, time_t> modFiles;

public:


	IOStuff() {
		state = luaL_newstate();
		luaL_openlibs(state);
		dirpath = IOStuff::getexepath();
		using namespace LuaToCpp;

		lua_pushcfunction(state, lua_sleep);
		lua_setglobal(state, "sleep");
	}
	~IOStuff() {
		lua_close(state);
	}

	static std::string getexepath()
	{
		char result[MAX_PATH];
		std::string filepath = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
		while (filepath.size() > 0 && filepath[filepath.size() - 1] != '\\') {
			filepath = filepath.substr(0, filepath.size() - 1);
		}
		return filepath;
	}

	bool isFileModified(std::string filename) {
		struct stat result;
		std::string dirpath = IOStuff::getexepath() + filename;
		if (stat(dirpath.c_str(), &result) == 0)
		{
			time_t mod_time = result.st_mtime;

			std::map<std::string, time_t>::iterator lb = modFiles.lower_bound(filename);

			if (lb != modFiles.end())
			{
				// key already exists
				time_t oldTime = lb->second;
				modFiles[filename] = mod_time;
				return oldTime != mod_time;
			}
			else
			{
				// the key does not exist in the map
				modFiles.insert(lb, std::map<std::string, time_t>::value_type(filename, mod_time));
				return true;
			}

		}
		return false;
	}

	/*static long getFileSize(std::string filename)
	{
		struct stat stat_buf;
		std::string dirpath = IOStuff::getexepath() + filename;
		int rc = stat(dirpath.c_str(), &stat_buf);
		return rc == 0 ? stat_buf.st_size : -1;
	}*/

	void scriptNoArgs(std::string filename) {
		std::string scripts = dirpath + filename;
		int error = luaL_loadfile(state, scripts.c_str());
		if (error == LUA_ERRFILE) {
			std::cout << "READ ERROR!" << std::endl;
			return;
		}
		lua_pcall(state, 0, LUA_MULTRET, 0);
	}


};