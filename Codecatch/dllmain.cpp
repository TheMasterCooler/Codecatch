// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>

#include "codecatch.h"
#include "console.h"
#include "file.h"

int Main() {
	/* Setup the console */
	CC::Console::Init();
	/**/
	
	/* Setup the file handler */
	CC::File::Init();
	/**/

	/* Setup detours */
	if (CC::Init()) CC::Console::Print("Hooking successful.\n", COLOUR_GREEN);
	else CC::Console::Print("Hooking failed.\n", COLOUR_DARKRED);
	/**/

	return 1;
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, hModule, 0, nullptr);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

