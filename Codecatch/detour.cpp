#include <Windows.h>

#include "detour.h"

// stolen from leystryuku https://github.com/Leystryku/katazuro/blob/master/katazuro/katazuro.cpp

#define JMP32_SZ 5
#define BIT32_SZ 4
#define SIG_SZ 3
#define SIG_OP_0 0xCC
#define SIG_OP_1 0x90
#define SIG_OP_2 0xC3

namespace CC {
	LPVOID DetourCreate(LPVOID lpFuncOrig, LPVOID lpFuncDetour, int detourLen) {
		LPVOID lpMallocPtr = NULL;
		DWORD dwProt = NULL;
		PBYTE pbMallocPtr = NULL;
		PBYTE pbFuncOrig = (PBYTE)lpFuncOrig;
		PBYTE pbFuncDetour = (PBYTE)lpFuncDetour;
		PBYTE pbPatchBuf = NULL;
		int minDetLen = 5;
		int detLen = detourLen;

		// Alloc mem for the overwritten bytes
		if ((lpMallocPtr = VirtualAlloc(NULL, detLen + JMP32_SZ + SIG_SZ, MEM_COMMIT, PAGE_EXECUTE_READWRITE)) == NULL)
			return NULL;

		pbMallocPtr = (PBYTE)lpMallocPtr;

		// Enable writing to original
		VirtualProtect(lpFuncOrig, detLen, PAGE_READWRITE, &dwProt);

		// Write overwritten bytes to the malloc
		memcpy(lpMallocPtr, lpFuncOrig, detLen);
		pbMallocPtr += detLen;
		pbMallocPtr[0] = 0xE9;
		*(DWORD*)(pbMallocPtr + 1) = (DWORD)((pbFuncOrig + detLen) - pbMallocPtr) - JMP32_SZ;
		pbMallocPtr += JMP32_SZ;
		pbMallocPtr[0] = SIG_OP_0;
		pbMallocPtr[1] = SIG_OP_1;
		pbMallocPtr[2] = SIG_OP_2;

		// Create a buffer to prepare the detour bytes
		pbPatchBuf = new BYTE[detLen];
		memset(pbPatchBuf, 0x90, detLen);

		pbPatchBuf[0] = 0xE9;
		*(DWORD*)& pbPatchBuf[1] = (DWORD)(pbFuncDetour - pbFuncOrig) - 5;


		// Write the detour
		for (int i = 0; i < detLen; i++)
			pbFuncOrig[i] = pbPatchBuf[i];

		delete[] pbPatchBuf;

		// Reset original mem flags
		DWORD dwOldProt;
		VirtualProtect(lpFuncOrig, detLen, dwProt, &dwOldProt);

		return lpMallocPtr;
	}
}