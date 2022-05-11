#include <Windows.h>
#include <iostream>
#include <vector>

#include "Lua/LuaInterface.h"
#include "Lua/ILuaShared.h"

#include "codecatch.h"
#include "detour.h"
#include "console.h"
#include "file.h"

typedef void* (__cdecl* tLuaL_loadbuffer)(void* state, const char* buff, size_t sz, const char* name);
tLuaL_loadbuffer luaL_loadbuffer;

typedef void* (__cdecl* tLuaL_loadbufferx)(void* state, const char* buff, size_t sz, const char* name, void* mode);
tLuaL_loadbufferx luaL_loadbufferx;

typedef void* (__cdecl* tLuaL_loadstring)(void* state, const char* str);
tLuaL_loadstring luaL_loadstring;

typedef void* (__cdecl* tLua_pcall)(void* state, int num_args, int num_results, int error_func);
tLua_pcall lua_pcall;

typedef void* (__cdecl* tLua_toboolean)(void* state, int stack_index);
tLua_toboolean lua_toboolean;

void* __cdecl luaL_loadbufferx_hook(void* state, const char* buff, size_t sz, const char* path, void* mode) {
	std::string sPath = path;
	sPath.erase(0, 1); // Remove the @ at the beginning. Bodge I know but idgaf.

	CC::Console::Print("Intercepted: " + sPath + "\n", COLOUR_GRAY);

	std::string sBuff = buff;

	CC::File::SaveCode(sPath, sBuff);

	if (sPath == "lua/includes/init.lua") {
		luaL_loadstring(state, "print('ok boomer') return false");
		lua_pcall(state, 0, 1, 0);
		bool succ = lua_toboolean(state, -1);

		if (succ) CC::Console::Print("True yes.\n");
		else CC::Console::Print("False no.\n");
	}

	return luaL_loadbuffer(state, buff, sz, path); // Return the other loadbuffer function to load the code.
}

namespace CC {
	int Init() {
		HMODULE LuaShared = GetModuleHandleA("lua_shared.dll"); // Find lua_shared.dll
		if (!LuaShared) {
			CC::Console::Print("Module 'lua_shared.dll' not found.\n", COLOUR_RED);
			return 0;
		}
		
		void* luaL_loadbufferx_address = GetProcAddress(LuaShared, "luaL_loadbufferx"); // Find the address of the function we are going to detour.
		if (!luaL_loadbufferx_address) {
			CC::Console::Print("Address 'luaL_loadbufferx' not found in 'lua_shared.dll'.\n", COLOUR_RED);
			return 0;
		}

		luaL_loadbuffer = (tLuaL_loadbuffer)GetProcAddress(LuaShared, "luaL_loadbuffer"); // Find the address of the function we will run to run code.
		if (!luaL_loadbuffer) {
			CC::Console::Print("Address for 'luaL_loadbuffer' not found in 'lua_shared.dll'.\n", COLOUR_RED);
			return 0;
		}

		luaL_loadstring = (tLuaL_loadstring)GetProcAddress(LuaShared, "luaL_loadstring");
		if (!luaL_loadstring) {
			CC::Console::Print("Address for 'luaL_loadstring' not found in 'lua_shared.dll'.\n", COLOUR_RED);
			return 0;
		}

		lua_pcall = (tLua_pcall)GetProcAddress(LuaShared, "lua_pcall");
		if (!lua_pcall) {
			CC::Console::Print("Address for 'lua_pcall' not found in 'lua_shared.dll'.\n", COLOUR_RED);
			return 0;
		}

		lua_toboolean = (tLua_toboolean)GetProcAddress(LuaShared, "lua_toboolean");
		if (!lua_toboolean) {
			CC::Console::Print("Address for 'lua_toboolean' not found in 'lua_shared.dll'.\n", COLOUR_RED);
			return 0;
		}

		luaL_loadbufferx = (tLuaL_loadbufferx)DetourCreate(luaL_loadbufferx_address, luaL_loadbufferx_hook, 6); // Detour that shit.
		if (!luaL_loadbufferx) {
			CC::Console::Print("Failed to detour 'luaL_loadbufferx' in 'lua_shared.dll'.\n", COLOUR_RED);
			return 0;
		}

		return 1;
	}
}