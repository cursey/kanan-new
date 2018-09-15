#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "CursorUnhide.hpp"

using namespace std;

namespace kanan {
    CursorUnhide::CursorUnhide()
        : m_isEnabled{ false },
        m_patch{}
    {
        auto address = scan("client.exe", "68 B5 00 00 00 50 FF D7 89 86 70 01 00 00 8B 0D");
        if (address) {
            log("Got CursorUnhide %p", *address);
            m_patch.address = *address + 1; // offset

        }
        else {
            log("Failed to find CursorUnhide address");
        }
    }

    void CursorUnhide::onUI() {
        if (ImGui::CollapsingHeader("Chain Cursor Unhide")) {
            ImGui::TextWrapped(
                "Prevents the game from hiding the 2D cursor when displaying the 3D cursor for chain skills."
                "NOTE: You must restart the game after enabling or disabling this mod for it take effect."
            );
            ImGui::Spacing();

            if (ImGui::Checkbox("Chain Cursor Unhide", &m_isEnabled)) {
                apply();
            }
        }
    }

    void CursorUnhide::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("CursorUnhide.Enabled").value_or(false);

        if (m_isEnabled) {
            apply();
        }
    }

    void CursorUnhide::onConfigSave(Config& cfg) {
        cfg.set<bool>("CursorUnhide.Enabled", m_isEnabled);
    }

    void CursorUnhide::apply() {
        if (!m_isEnabled || m_patch.address == 0) {
            return;
        }
        m_patch.bytes = { 65 };

        log("Applying Cursor Unhide...");
        patch(m_patch);
    }
}