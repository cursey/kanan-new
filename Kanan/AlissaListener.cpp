#include <array>

#include <imgui.h>

#include <String.hpp>

#include "Kanan.hpp"
#include "Log.hpp"
#include "AlissaListener.hpp"

using std::array;
using std::string;
using std::move;

namespace kanan {
    BOOL CALLBACK enumWindow(HWND wnd, LPARAM param) {
        auto alissaListener = (AlissaListener*)param;
        array<wchar_t, 256> name{};

        if (GetWindowText(wnd, name.data(), (int)name.size()) == 0) {
            return TRUE;
        }

        // Skip non-alissa windows.
        if (narrow(name.data()) != "mod_Alissa") {
            return TRUE;
        }

        if (GetClassName(wnd, name.data(), (int)name.size()) == 0) {
            return TRUE;
        }

        alissaListener->addAlissaProvider(wnd, narrow(name.data()));

        return TRUE;
    }

    AlissaListener::AlissaListener()
        : m_isEnabled{ false },
        m_isConnected{ false },
        m_isConnectUIOpen{ false },
        m_alissaProviders{},
        m_alissaProvider{}
    {
    }

    void AlissaListener::onUI() {
        if (ImGui::CollapsingHeader("[WIP] Alissa Listener")) {
            ImGui::Checkbox("Enable Alissa Listener", &m_isEnabled);

            if (m_isEnabled) {
                if (m_isConnected) {
                    if (ImGui::Button("Disconnect")) {
                        disconnect();
                    }
                }
                else {
                    if (ImGui::Button("Connect")) {
                        findAlissaProviders();
                        m_isConnectUIOpen = true;
                    }
                }
            }
        }

        // TODO: Only show the connect UI when there is more than one Alissa provider
        // otherwise just connect to the only instance.
        if (m_isConnectUIOpen) {
            connectUI();
        }
    }

    void AlissaListener::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("AlissaListener.Enabled").value_or(false);
    }

    void AlissaListener::onConfigSave(Config& cfg) {
        cfg.set<bool>("AlissaListener.Enabled", m_isEnabled);
    }

    bool AlissaListener::onMessage(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) {
        // Skip if we're not enabled or not connected.
        if (!m_isEnabled || !m_isConnected) {
            return true;
        }

        // Skip if the message isn't WM_COPYDATA.
        if (message != WM_COPYDATA) {
            return true;
        }

        auto cds = (COPYDATASTRUCT*)lParam;

        switch ((Sign)cds->dwData) {
        case Sign::SEND: 
            log("[AlissaListener] Sent %d byte packet", cds->cbData);
            break;

        case Sign::RECV: 
            log("[AlissaListener] Recieved %d byte packet", cds->cbData);
            break;

        default:
            break;
        }

        return true;
    }

    void AlissaListener::addAlissaProvider(HWND wnd, const string& name) {
        m_alissaProviders.emplace_back(AlissaProvider{ wnd, name });
        log("[AlissaListener] Found Alissa provider %s", name.c_str());
    }

    void AlissaListener::connectUI() {
        ImGui::OpenPopup("Select Alissa Provider");
        ImGui::SetNextWindowSize(ImVec2{ 400.0f, 200.0f }, ImGuiSetCond_Always);

        if (!ImGui::BeginPopupModal("Select Alissa Provider", &m_isConnectUIOpen, ImGuiWindowFlags_NoResize)) {
            return;
        }

        if (ImGui::BeginCombo("Provider", m_alissaProvider.name.c_str())) {
            int id{};

            for (auto& provider : m_alissaProviders) {
                ImGui::PushID(id++);

                auto isSelected = provider.wnd == m_alissaProvider.wnd;

                if (ImGui::Selectable(provider.name.c_str(), isSelected)) {
                    m_alissaProvider = provider;
                }

                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }

                ImGui::PopID();
            }

            ImGui::EndCombo();
        }

        if (ImGui::Button("Connect")) {
            connect();
        }

        if (m_isConnected) {
            // Close the connect UI if we're connected now.
            m_isConnectUIOpen = false;
        }

        ImGui::EndPopup();
    }

    void AlissaListener::findAlissaProviders() {
        log("[AlissaListener] Looking for Alissa providers...");

        m_alissaProviders.clear();

        EnumWindows(&enumWindow, (LPARAM)this);

        log("[AlissaListener] Finished looking for Alissa providers.");
    }

    void AlissaListener::connect() {
        // Can't connect if we're already connected or the alissa provider isn't
        // valid.
        if (m_isConnected || m_alissaProvider.name.empty()) {
            return;
        }

        log("[AlissaListener] Connecting to Alissa provider %s...", m_alissaProvider.name.c_str());

        COPYDATASTRUCT cds{};
        cds.dwData = (ULONG_PTR)Sign::CONNECT;

        SendMessage(m_alissaProvider.wnd, WM_COPYDATA, (WPARAM)g_kanan->getWindow(), (LPARAM)&cds);

        m_isConnected = true;

        log("[AlissaListener] Finished connecting to Alissa provider.");
    }

    void AlissaListener::disconnect() {
        // Can't disconnect if we're already disconnected.
        if (!m_isConnected) {
            return;
        }

        log("[AlissaListener] Disconnecting from Alissa provider %s...", m_alissaProvider.name.c_str());

        COPYDATASTRUCT cds{};
        cds.dwData = (ULONG_PTR)Sign::DISCONNECT;

        SendMessage(m_alissaProvider.wnd, WM_COPYDATA, (WPARAM)g_kanan->getWindow(), (LPARAM)&cds);

        m_isConnected = false;

        log("[AlissaListener] Finished disconnecting from Alissa provider.");
    }
}