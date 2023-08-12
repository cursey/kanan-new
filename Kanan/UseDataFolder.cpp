#include <imgui.h>

#include <Scan.hpp>
#include <Utility.hpp>

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

        auto address = scan("client.exe", "48 8D 0D ? ? ? ? E8 ? ? ? ? 84 C0 74 ? 48 8B 0D ? ? ? ? 4C 8B C3");

        if (address) {
            m_fileSystem = (uintptr_t)rel_to_abs(*address + 3);

            log("[UseDataFolder] Got address of CFileSystem %p", m_fileSystem);
        }
        else {
            log("[UseDataFolder] Failed to get address of CFileSystem");
        }

        address = scan("client.exe", "E8 ? ? ? ? 48 8B ? ? E8 ? ? ? ? 48 8B ? ? ? 48 8B ? ? ? 48 8B ? ? ? 48 83 C4 ? 41");//E8 ? ? ? ? 48 8B 4E 08 E8 ? ? ? ? 48 8B 5C 24 58

        if (address) {
            m_setLookUpOrder = (decltype(m_setLookUpOrder))rel_to_abs(*address + 1);

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
