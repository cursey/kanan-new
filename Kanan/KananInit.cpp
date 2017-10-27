#include <Windows.h>

#include "Log.hpp"
#include "Kanan.hpp"

using namespace std;
using namespace kanan;

//
// This is the entrypoint for kanan. It's only responsible for setting up the global
// log file and creating the global kanan object. 
//
extern "C" __declspec(dllexport) int kananInit(const char* path) {
    // First and most important thing is opening the log file.
    g_log.open(string{ path } +"/log.txt");

    if (!g_log.is_open()) {
        MessageBox(GetDesktopWindow(), L"Failed to open log.txt!", L"Kanan Error!", MB_ICONERROR | MB_OK);
    }

    g_log << "Welcome to Kanan for Mabinogi." << endl;
    g_log << "Creating Kanan object." << endl;

    g_kanan = make_unique<Kanan>(path);

    g_log << "Leaving kananInit." << endl;

    return 0;
}