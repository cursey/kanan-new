#pragma once

#include <string>

#include <Windows.h>

#include <Config.hpp>

namespace kanan {
    class Mod {
    public:
        virtual void onFrame() {}

        virtual void onUI() {}
        virtual void onPatchUI() {}

        virtual void onConfigLoad(const Config& cfg) {}
        virtual void onConfigSave(Config& cfg) {}

        virtual bool onMessage(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) { return true; }

        virtual void onKeyDown(DWORD key) {}
        virtual void onKeyUp(DWORD key) {}
    };
}
