#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "UseDataFolder.hpp"

using namespace std;

namespace kanan {
    UseDataFolder::UseDataFolder()
        : m_isEnabled{ false },
        m_fileSystem{ 0 },
        m_setLookUpOrder{ nullptr }
    {
        log("[UseDataFolder] Entering constructor...");

        auto address = scan("client.exe", "B9 ? ? ? ? E8 ? ? ? ? 80 7B 13 ?");

        if (address) {
            m_fileSystem = *(uintptr_t*)(*address + 1);

            log("[UseDataFolder] Got address of CFileSystem %p", m_fileSystem);
        }
        else {
            log("[UseDataFolder] Failed to get address of CFileSystem");
        }

        address = scan("client.exe", "55 8B EC 6A ? 68 ? ? ? ? 64 A1 ? ? ? ? 50 83 EC ? 53 56 57 A1 ? ? ? ? 33 C5 50 8D 45 F4 64 A3 ? ? ? ? 8B F1 89 75 EC E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8B 46 04");

        if (address) {
            m_setLookUpOrder = (decltype(m_setLookUpOrder))*address;

            log("[UseDataFolder] Got address of SetLookUpOrder %p", m_setLookUpOrder);
        }
        else {
            log("[UseDataFolder] Failed to get address of SetLookUpOrder");
        }

        log("[UseDataFolder] Leaving constructor.");
    }

    void UseDataFolder::onUI() {
        if (ImGui::CollapsingHeader("Use Data Folder")) {
            ImGui::TextWrapped(
                "Causes the game to load its files from the /data/ directory letting you modify the files Mabinogi uses. "
                "NOTE: You must restart the game after enabling or disabling this mod for it take effect."
            );
            ImGui::Spacing();

            if (ImGui::Checkbox("Enable Data Folder", &m_isEnabled)) {
                apply();
            }
        }
    }

    void UseDataFolder::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("UseDataFolder.Enabled").value_or(false);

        if (m_isEnabled) {
            apply();
        }
    }

    void UseDataFolder::onConfigSave(Config& cfg) {
        cfg.set<bool>("UseDataFolder.Enabled", m_isEnabled);
    }

    void UseDataFolder::apply() {
        if (!m_isEnabled || m_fileSystem == 0 || m_setLookUpOrder == nullptr) {
            return;
        }

        log("[UseDataFolder] Toggling on");

        m_setLookUpOrder(m_fileSystem, 0);
    }
}
