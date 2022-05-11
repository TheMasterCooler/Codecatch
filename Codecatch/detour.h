#pragma once

#include <Windows.h>

namespace CC {
	LPVOID DetourCreate(LPVOID lpFuncOrig, LPVOID lpFuncDetour, int detourLen);
}