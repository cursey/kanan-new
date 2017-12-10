#pragma once

#include <memory>
#include <string>
#include <mutex>
#include <condition_variable>

#include <Windows.h>

#include "D3D9Hook.hpp"
#include "DInputHook.hpp"
#include "WindowsMessageHook.hpp"
#include "Game.hpp"
#include "Mods.hpp"

namespace kanan {
    // This is the main object that sets up all the patches and stuff.
    class Kanan {
    public:
        Kanan(std::string path);

        auto getGame() const {
            return m_game.get();
        }

        auto getMods() const {
            return m_mods.get();
        }

        auto getWindow() const {
            return m_wnd;
        }
		void onInject();

    private:
        std::string m_path;
        std::string m_uiConfigPath;
        std::unique_ptr<D3D9Hook> m_d3d9Hook;
        std::unique_ptr<DInputHook> m_dinputHook;
        std::unique_ptr<WindowsMessageHook> m_wmHook;
        std::unique_ptr<Game> m_game;
        std::unique_ptr<Mods> m_mods;
        bool m_isUIOpen;
        
		std::mutex m_mabiLoadLock;
		std::condition_variable m_mabiReady;
		bool m_isModsLoaded;
		bool m_isInitialized;
        HWND m_wnd;

		void initializeMods();
        // These are callbacks that get called from the hooks that get created.
        void onInitialize();
        void onFrame();
        bool onMessage(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam);

        void loadConfig();
        void saveConfig();
    };

    extern std::unique_ptr<Kanan> g_kanan;
}
