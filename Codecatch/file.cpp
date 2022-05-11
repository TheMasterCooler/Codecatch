#include <Windows.h>
#include <fstream>

#include "Lua/LuaInterface.h"
#include "Lua/ILuaShared.h"

#include "file.h"
#include "console.h"

namespace CC {
	namespace File {
		char gmod_path[MAX_PATH];
		std::string dumpDir;

		int Init() {
			if (!GetModuleFileNameA((HMODULE)GetModuleHandleA("gmod.exe"), gmod_path, sizeof(gmod_path))) {
				CC::Console::Print("Failed to find the path to 'gmod.exe'.\n");
				return 0;
			}

			return 1;
		}

		void SaveCode(std::string fileName, std::string code) {
			std::string luaDir;
			std::string luaFile;

			std::vector<std::string> folders = SplitDirectory(fileName, "/");
			luaFile = folders.back(); folders.pop_back();
			luaDir = "G:/Stolen";

			for (int i = 0; i < folders.size(); i++) {
				std::string newDir = (luaDir + "/" + folders.at(i));
				CreateDirectoryA(newDir.c_str(), NULL);
				luaDir = newDir;
			}

			std::ofstream luaOut(luaDir + "/" + luaFile);
			luaOut << code;
			luaOut.close();
		}

		std::vector<std::string> SplitDirectory(std::string dir, std::string separator) {
			std::vector<std::string> split;
			int lastBreak = 0;

			for (int i = 0; i < dir.length(); i++) {
				std::string cChar = dir.substr(i, 1);

				if (cChar == separator) {
					split.push_back(dir.substr(lastBreak, (i - lastBreak)));
					lastBreak = i + 1;
				}
			}

			split.push_back(dir.substr(lastBreak, dir.length() - lastBreak));

			return split;
		}
	}
}