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
    startLog(string{ path } + "/log.txt");

    log("Welcome to Kanan for Mabinogi.");
    log("Creating Kanan object.");

    g_kanan = make_unique<Kanan>(path);

    log("Leaving kananInit.");

    return 0;
}