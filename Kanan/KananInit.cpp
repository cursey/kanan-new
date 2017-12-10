#include <Windows.h>

#include "Log.hpp"
#include "Kanan.hpp"

using namespace std;
using namespace kanan;

char path[MAX_PATH];
//
// This is the entrypoint for kanan. It's only responsible for setting up the global
// log file and creating the global kanan object.
//
extern "C" __declspec(dllexport) DWORD WINAPI kananInit(LPVOID param) {
    // First and most important thing is opening the log file.
	string dllStr = string{ path };
	string pathStr = dllStr.substr(0, dllStr.find_last_of("\\/"));

    startLog(pathStr + "/log.txt");

    log("Welcome to Kanan for Mabinogi.");
    log("Creating Kanan object.");

    g_kanan = make_unique<Kanan>(pathStr);
	g_kanan->onInject();

    log("Leaving kananInit.");

    return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		GetModuleFileNameA(hInst, path, sizeof(path));
		DisableThreadLibraryCalls(hInst);
		DWORD  dwThreadId;
		HANDLE hThread = CreateThread(
			NULL, 0, kananInit, NULL, 0, &dwThreadId);
	}
	return TRUE;

	case DLL_PROCESS_DETACH:
		return TRUE;
	}
	return FALSE;
}