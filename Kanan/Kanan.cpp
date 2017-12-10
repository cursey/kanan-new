#include <imgui.h>
#include <imgui_impl_dx9.h>

#include <Config.hpp>
#include <String.hpp>
#include <Utility.hpp>

#include "Log.hpp"
#include "Kanan.hpp"

using namespace std;

namespace kanan {
    unique_ptr<Kanan> g_kanan{ nullptr };

    Kanan::Kanan(string path)
        : m_path{ move(path) },
        m_uiConfigPath{ m_path + "/ui.ini" },
        m_d3d9Hook{ nullptr },
        m_dinputHook{ nullptr },
        m_wmHook{ nullptr },
        m_game{ nullptr },
        m_mods{ nullptr },
        m_isUIOpen{ true },
        m_isInitialized{ false },
		m_isModsLoaded{ false },
        m_wnd{ nullptr }
    {
        log("Entering Kanan constructor.");

        //
        // Hook D3D9 and set the callbacks.
        //
        log("Hooking D3D9...");

        m_d3d9Hook = make_unique<D3D9Hook>();

        m_d3d9Hook->onPresent = [this](auto&) { onFrame(); };
        m_d3d9Hook->onPreReset = [](auto&) { ImGui_ImplDX9_InvalidateDeviceObjects(); };
        m_d3d9Hook->onPostReset = [](auto&) { ImGui_ImplDX9_CreateDeviceObjects(); };

        if (!m_d3d9Hook->isValid()) {
            error("Failed to hook D3D9.");
        }
		std::lock_guard<std::mutex> mabiLock(m_mabiLoadLock);

        log("Leaving Kanan constructor.");
    }

	void Kanan::onInject() {
		std::unique_lock<std::mutex> lock(m_mabiLoadLock);
		m_mabiReady.wait(lock);
		initializeMods();
		m_isModsLoaded = true;
	}

	void Kanan::initializeMods() {
		//
		// Initialize the Game object.
		//
		log("Creating the Game object...");

		m_game = make_unique<Game>();

		//
		// Initialize all the mods.
		//
		log("Creating Mods object...");

		m_mods = make_unique<Mods>();

		log("Calling Mod::onInitialize callbacks...");

		for (const auto& mod : m_mods->getMods()) {
			mod->onInitialize();
		}

		//
		// Load the config.
		//
		log("Loading config...");

		loadConfig();

		log("Done initializing.");
	}

    void Kanan::onInitialize() {
        log("Begginging intialization... ");

        // Grab the HWND from the device's creation parameters.
        log("Getting window from D3D9 device...");

        auto device = m_d3d9Hook->getDevice();
        D3DDEVICE_CREATION_PARAMETERS creationParams{};

        device->GetCreationParameters(&creationParams);

        m_wnd = creationParams.hFocusWindow;

        //
        // ImGui.
        //
        log("Initializing ImGui...");

        ImGui::GetIO().IniFilename = m_uiConfigPath.c_str();

        if (!ImGui_ImplDX9_Init(m_wnd, device)) {
            error("Failed to initialize ImGui.");
        }

        // These render strangely in Mabinogi so we disable them.
        auto& style = ImGui::GetStyle();

        style.AntiAliasedLines = false;
        style.AntiAliasedShapes = false;
        style.WindowRounding = 0.0f;
        style.ChildWindowRounding = 0.0f;
        style.FrameRounding = 0.0f;
        style.ScrollbarRounding = 0.0f;
        style.GrabRounding = 0.0f;

        //
        // DInputHook.
        //
        log("Hooking DInput...");

        m_dinputHook = make_unique<DInputHook>(m_wnd);

        if (!m_dinputHook->isValid()) {
            error("Failed to hook DInput.");
        }

        //
        // WindowsMessageHook.
        //
        log("Hooking the windows message procedure...");

        m_wmHook = make_unique<WindowsMessageHook>(m_wnd);

        m_wmHook->onMessage = [this](auto wnd, auto msg, auto wParam, auto lParam) {
            return onMessage(wnd, msg, wParam, lParam);
        };

        if (!m_wmHook->isValid()) {
            error("Failed to hook windows message procedure.");
        }
		m_mabiReady.notify_all();
    }

    void Kanan::onFrame() {
        if (!m_isInitialized) {
            onInitialize();
            m_isInitialized = true;
        }

        ImGui_ImplDX9_NewFrame();

		if (!m_isModsLoaded) {
			ImGui::OpenPopup("Loading...");
			ImGui::SetNextWindowPosCenter();
			ImGui::SetNextWindowSize(ImVec2{ 450.0f, 200.0f }, ImGuiSetCond_FirstUseEver);
			if (ImGui::BeginPopupModal("Loading...")) {
				ImGui::Text("Kanan is currently setting up. Please wait a moment...");
				ImGui::EndPopup();
			}

			m_dinputHook->ignoreInput();

			auto device = m_d3d9Hook->getDevice();

			device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

			ImGui::Render();
			
			return;
		}

        for (const auto& mod : m_mods->getMods()) {
            mod->onFrame();
        }

        if (wasKeyPressed(VK_INSERT)) {
            m_isUIOpen = !m_isUIOpen;

            // Save the config whenever the menu closes.
            if (!m_isUIOpen) {
                saveConfig();
            }
        }

        if (m_isUIOpen) {
            m_dinputHook->ignoreInput();


            ImGui::SetNextWindowSize(ImVec2{ 450.0f, 200.0f }, ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Kanan's New Mabinogi Mod");
            ImGui::Text("Input to the game is blocked while this window is open!");

            if (ImGui::CollapsingHeader("Patches")) {
                for (const auto& mod : m_mods->getMods()) {
                    mod->onPatchUI();
                }
            }

            for (const auto& mod : m_mods->getMods()) {
                mod->onUI();
            }

            ImGui::End();

        }
        else {
            m_dinputHook->acknowledgeInput();
        }

        // This fixes mabi's Film Style Post Shader making ImGui render as a black box.
        auto device = m_d3d9Hook->getDevice();

        device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
        device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

        ImGui::Render();
    }

    bool Kanan::onMessage(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (m_isUIOpen) {
            if (ImGui_ImplDX9_WndProcHandler(wnd, message, wParam, lParam) != 0) {
                return false;
            }
        }

        return true;
    }

    void Kanan::loadConfig() {
        if (!m_mods) {
            return;
        }

        log("Loading config %s/config.txt", m_path.c_str());

        Config cfg{ m_path + "/config.txt" };

        for (auto& mod : m_mods->getMods()) {
            mod->onConfigLoad(cfg);
        }

        log("Config loading done.");
    }

    void Kanan::saveConfig() {
        if (!m_mods) {
            return;
        }

        log("Saving config %s/config.txt", m_path.c_str());

        Config cfg{};

        for (auto& mod : m_mods->getMods()) {
            mod->onConfigSave(cfg);
        }

        if (!cfg.save(m_path + "/config.txt")) {
            log("Failed to save the config %s/config.txt", m_path.c_str());
        }

        log("Config saving done.");
    }
}
