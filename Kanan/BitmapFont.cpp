#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "BitmapFont.hpp"

using namespace std;

namespace kanan {
    static BitmapFont* g_bitmapFont;

    BitmapFont::BitmapFont()
        : m_isEnabled{ false },
        m_patches{}
    {
        // Patterns from Abyss by Blade3575.
        auto address0 = scan("client.exe", "74 0D 8B 45 08 50 8B CB E8 ? ? ? ? EB 0B");
        m_patches[0].address = address0.value_or(0);
        m_patches[0].bytes = { 0x90, 0x90 };

        auto address1 = scan("client.exe", "75 3D 83 BE ? ? ? ? ? ");
        m_patches[1].address = address1.value_or(0);
        m_patches[1].bytes = { 0xEB };

        auto address2 = scan("client.exe", "74 25 80 7B 73 00");
        m_patches[2].address = address2.value_or(0);
        m_patches[2].bytes = { 0xEB };

        auto address3 = scan("client.exe", "0F 84 ? ? ? ? 8B 0D ? ? ? ? E8 ? ? ? ? 57");
        m_patches[3].address = address3.value_or(0);
        m_patches[3].bytes = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, -1, -1, -1, -1, -1, -1, 0x90, 0x90, 0x90, 0xB0, 0x01, -1, -1, -1, -1, 0xEB };

        if (address0 && address1 && address2 && address3) {
            log("Found all the addressess required for BitmapFont.");
        }
        else {
            log("Failed to find one or more addresses for BitmapFont!");
        }
    }

    void BitmapFont::onPatchUI() {
        // Don't display the UI if any of the patches don't have an address.
        for (auto& p : m_patches) {
            if (p.address == 0) {
                return;
            }
        }

        if (ImGui::Checkbox("Use Bitmap Font", &m_isEnabled)) {
            apply();
        }

        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Credits: Blade3575");
        }
    }

    void BitmapFont::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("BitmapFont.Enabled").value_or(false);

        if (m_isEnabled) {
            apply();
        }
    }

    void BitmapFont::onConfigSave(Config& cfg) {
        cfg.set<bool>("BitmapFont.Enabled", m_isEnabled);
    }

    void BitmapFont::apply() {
        // Don't apply any patches if any of them don't have an address.
        for (auto& p : m_patches) {
            if (p.address == 0) {
                return;
            }
        }

        log("Toggling BitmapFont...");

        if (m_isEnabled) {
            for (auto& p : m_patches) {
                patch(p);
            }
        }
        else {
            for (auto& p : m_patches) {
                undoPatch(p);
            }
        }
    }
}
