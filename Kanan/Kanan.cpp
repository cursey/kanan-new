#include <filesystem>

#include <cpptoml.h>

#include <imgui.h>
#include <imgui_impl_dx9.h>

#include <String.hpp>
#include <Utility.hpp>

#include "Log.hpp"
#include "Kanan.hpp"

using namespace std;
using namespace std::experimental::filesystem;

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
        m_wnd{ nullptr }
    {
        g_log << "Entering Kanan constructor." << endl;

        //
        // Hook D3D9 and set the callbacks.
        //
        g_log << "Hooking D3D9..." << endl;

        m_d3d9Hook = make_unique<D3D9Hook>();

        m_d3d9Hook->onPresent = [this](auto&) { onFrame(); };
        m_d3d9Hook->onPreReset = [](auto&) { ImGui_ImplDX9_InvalidateDeviceObjects(); };
        m_d3d9Hook->onPostReset = [](auto&) { ImGui_ImplDX9_CreateDeviceObjects(); };

        if (!m_d3d9Hook->isValid()) {
            errorMsg("Failed to hook D3D9.");
        }

        g_log << "Leaving Kanan constructor." << endl;
    }

    void Kanan::onInitialize() {
        g_log << "Begginging intialization... " << endl;

        // Grab the HWND from the device's creation parameters.
        g_log << "Getting window from D3D9 device..." << endl;

        auto device = m_d3d9Hook->getDevice();
        D3DDEVICE_CREATION_PARAMETERS creationParams{};

        device->GetCreationParameters(&creationParams);

        m_wnd = creationParams.hFocusWindow;

        //
        // ImGui.
        //
        g_log << "Initializing ImGui..." << endl;

        ImGui::GetIO().IniFilename = m_uiConfigPath.c_str();

        if (!ImGui_ImplDX9_Init(m_wnd, device)) {
            errorMsg("Failed to initialize ImGui.");
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
        g_log << "Hooking DInput..." << endl;

        m_dinputHook = make_unique<DInputHook>(m_wnd);

        if (!m_dinputHook->isValid()) {
            errorMsg("Failed to hook DInput.");
        }

        // 
        // WindowsMessageHook.
        //
        g_log << "Hooking the windows message procedure..." << endl;

        m_wmHook = make_unique<WindowsMessageHook>(m_wnd);

        m_wmHook->onMessage = [this](auto wnd, auto msg, auto wParam, auto lParam) {
            return onMessage(wnd, msg, wParam, lParam);
        };

        if (!m_wmHook->isValid()) {
            errorMsg("Failed to hook windows message procedure.");
        }

        //
        // Initialize the Game object.
        //
        g_log << "Creating the Game object..." << endl;

        m_game = make_unique<Game>();
        
        //
        // Initialize all the mods.
        //
        g_log << "Creating Mods object..." << endl;

        m_mods = make_unique<Mods>();

        g_log << "Calling Mod::onInitialize callbacks..." << endl;

        for (const auto& mod : m_mods->getMods()) {
            mod->onInitialize();
        }

        // 
        // Load the config.
        // 
        g_log << "Loading config..." << endl;

        loadConfig();

        g_log << "Done initializing." << endl;
    }

    void Kanan::onFrame() {
        if (!m_isInitialized) {
            onInitialize();
            m_isInitialized = true;
        }

        ImGui_ImplDX9_NewFrame();

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

        // Only attempt loading the config if the config file actually exists.
        if (!exists(m_path + "/config.toml")) {
            return;
        }

        g_log << "Loading config " << m_path << "/config.toml" << endl;

        try {
            auto cfg = cpptoml::parse_file(m_path + "/config.toml");

            for (auto& mod : m_mods->getMods()) {
                mod->onConfigLoad(cfg);
            }

            g_log << "Config loading done." << endl;
        }
        catch (const cpptoml::parse_exception& e) {
            errorMsg(e.what());
        }
    }

    void Kanan::saveConfig() {
        if (!m_mods) {
            return;
        }

        g_log << "Saving config " << m_path << "/config.toml" << endl;

        auto cfg = cpptoml::make_table();

        for (auto& mod : m_mods->getMods()) {
            mod->onConfigSave(cfg);
        }

        ofstream cfgFile{ m_path + "/config.toml" };

        if (!cfgFile.is_open()) {
            errorMsg("Failed to open config.toml for writing!");
            return;
        }


        cfgFile << *cfg << endl;

        g_log << "Config saving done." << endl;
    }
}