#include <exception>

#include <Windows.h>

#include <String.hpp>

#include "LauncherApp.hpp"

using std::exception;
using kanan::widen;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int show) {
    try {
        LauncherApp app{};

        app.run();
    }
    catch (const exception& e) {
        MessageBox(GetDesktopWindow(), widen(e.what()).c_str(), L"Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    return 0;
}

