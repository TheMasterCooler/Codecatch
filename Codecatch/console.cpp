#include <Windows.h>
#include <iostream>

#include "console.h"

namespace CC {
	namespace Console {
		HANDLE handleOut;

		void Init() {
			AllocConsole();
			FILE* fp;
			fp = freopen("CONOUT$", "w", stdout);

			SetConsoleTitleA("CodeCatch");
			handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handleOut, COLOUR_DEFAULT);
		}
		
		void Print(std::string message, int colour) {
			SetConsoleTextAttribute(handleOut, COLOUR_YELLOW);
			std::cout << "[CodeCatch] ";

			SetConsoleTextAttribute(handleOut, colour);
			std::cout << message.c_str();

			SetConsoleTextAttribute(handleOut, COLOUR_DEFAULT);
		}
	}
}