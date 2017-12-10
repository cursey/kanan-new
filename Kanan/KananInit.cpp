#include <Windows.h>

#include <String.hpp>

#include "Log.hpp"
#include "Kanan.hpp"

using namespace std;
using namespace kanan;

TCHAR g_dllPath[MAX_PATH]{ 0 };

//
// This is the entrypoint for kanan. It's only responsible for setting up the global
// log file and creating the global kanan object.
//
DWORD WINAPI kananInit(LPVOID params) {
    // Convert g_dllPath to a path we can use.
    auto path = narrow(g_dllPath);

    path = path.substr(0, path.find_last_of("\\/"));

    // First and most important thing is opening the log file.
    startLog(path + "/log.txt");

    log("Welcome to Kanan for Mabinogi.");
    log("Creating Kanan object.");

    g_kanan = make_unique<Kanan>(path);

    log("Leaving kananInit.");

    return 0;
}

BOOL APIENTRY DllMain(HINSTANCE module, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        // We don't need DllMain getting invoked for thread attach/detach reasons.
        DisableThreadLibraryCalls(module);

        // Get the filepath of this dll.
        GetModuleFileName(module, g_dllPath, MAX_PATH);

        // Launch our init thread.
        CreateThread(nullptr, 0, kananInit, nullptr, 0, nullptr);
    }

    return TRUE;
}