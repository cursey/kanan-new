#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <Windows.h>

#include <Config.hpp>

namespace kanan {
    class MessageView;

    class Mod {
    public:
        virtual void onFrame() {}

        virtual void onUI() {}
        virtual void onPatchUI() {}

        virtual void onConfigLoad(const Config& cfg) {}
        virtual void onConfigSave(Config& cfg) {}

        virtual bool onMessage(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) { return true; }

        virtual void onSend(MessageView& msg) {}
        virtual void onRecv(MessageView& msg) {}
    };
}
