#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "EnableMultiClient.hpp"

using namespace std;

namespace kanan {
    EnableMultiClient::EnableMultiClient() 
        : m_isEnabled{ false },
        m_handlePtr{ nullptr }
    {
        log("Entering EnableMultiClient.");

        auto address = scan("client.exe", "B9 ? ? ? ? E8 ? ? ? ? 84 C0 74 ? 8B 0D ? ? ? ? 8D 55 C8");

        if (address) {
            log("Got EnableMultiClient %p", *address);

            m_handlePtr = (HANDLE**)(*address + 1);
        }
        else {
            log("Failed to find EnableMultiClient address.");
        }

        log("Leaving EnableMultiClient.");
    }

    void EnableMultiClient::onUI() {
        if (m_handlePtr == nullptr) {
            return;
        }

        if (ImGui::CollapsingHeader("Multi-Client")) {
            ImGui::Text("Enabling this feature lets you open more than one instance of Mabinogi.");
            ImGui::Dummy(ImVec2{ 10.0f, 1.0f });
            ImGui::Text("NOTE: You still need a launcher capable of launching Mabinogi even though");
            ImGui::Text("  an instance is already running.");
            ImGui::Dummy(ImVec2{ 10.0f, 1.0f });
            ImGui::Text("NOTE 2: Turning this feature off after it's been enabled requires a restart");
            ImGui::Text("  to take effect.");
            ImGui::Dummy(ImVec2{ 10.0f, 1.0f });

            if (ImGui::Checkbox("Enable Multi-Client", &m_isEnabled)) {
                apply();
            }
        }
    }

    void EnableMultiClient::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("EnableMultiClient.Enabled").value_or(false);

        if (m_isEnabled) {
            apply();
        }
    }

    void EnableMultiClient::onConfigSave(Config& cfg) {
        cfg.set<bool>("EnableMultiClient.Enabled", m_isEnabled);
    }

    void EnableMultiClient::apply() {
        if (m_handlePtr == nullptr) {
            return;
        }

        if (!m_isEnabled) {
            return;
        }

        log("Toggling EnableMultiClient...");

        if (SetHandleInformation(**m_handlePtr, HANDLE_FLAG_PROTECT_FROM_CLOSE, 0) == FALSE) {
            log("Failed to SetHandleInformation on the handle %p", *m_handlePtr);
            return;
        }

        if (CloseHandle(**m_handlePtr) == FALSE) {
            log("Failed to CloseHandle on the handle %p", *m_handlePtr);
            return;
        }

        log("Toggled EnableMultiClient on successfully.");
    }
}