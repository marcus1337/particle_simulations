#pragma once
#include <lua.hpp>
#include <windows.h>

namespace LuaToCpp {
	static int lua_sleep(lua_State *L)
	{
		int m = static_cast<int> (luaL_checknumber(L, 1));
		Sleep(m);
		return 0;
	}

}