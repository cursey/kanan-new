#pragma once

#include <vector>
#include <string>

#include <Windows.h>

#include "Mod.hpp"

namespace kanan {
    class AlissaListener : public Mod {
    public:
        AlissaListener();

        void onUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

        bool onMessage(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) override;

        void addAlissaProvider(HWND wnd, const std::string& name);

    private:
        enum class Sign : ULONG_PTR {
            CONNECT = 100,
            DISCONNECT = 101,
            SEND = 0x10101011,
            RECV = 0x10101012,
        };

        struct AlissaProvider {
            HWND wnd;
            std::string name;
        };

        bool m_isEnabled;
        bool m_isConnected;
        bool m_isConnectUIOpen;
        std::vector<AlissaProvider> m_alissaProviders;
        AlissaProvider m_alissaProvider; // The selected provider.

        void connectUI();
        void findAlissaProviders();

        void connect();
        void disconnect();
    };
}
