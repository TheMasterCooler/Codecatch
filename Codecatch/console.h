#pragma once

#define COLOUR_DARKBLUE		0x1
#define COLOUR_DARKGREEN	0x2
#define COLOUR_TEAL			0x3
#define COLOUR_DARKRED		0x4
#define COLOUR_MAGENTA		0x5
#define COLOUR_DYELLOW		0x6
#define COLOUR_DEFAULT		0x7
#define COLOUR_GRAY			0x8
#define COLOUR_BLUE			0x9
#define COLOUR_GREEN		0xA
#define COLOUR_CYAN			0xB
#define COLOUR_RED			0xC
#define COLOUR_PURPLE		0xD
#define COLOUR_YELLOW		0xE
#define COLOUR_WHITE		0xF

namespace CC {
	namespace Console {
		void Init();

		void Print(std::string message, int colour = COLOUR_DEFAULT);
	}
}