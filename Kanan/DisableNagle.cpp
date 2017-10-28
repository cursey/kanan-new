#include <WinSock2.h>

#include <imgui.h>

#include "Log.hpp"
#include "DisableNagle.hpp"

using namespace std;

namespace kanan {
    static DisableNagle* g_disableNagle;

    DisableNagle::DisableNagle()
        : m_isEnabled{ false },
        m_hook{ nullptr }
    {
        log("Entering DisableNagle constructor.");

        auto ws2_32 = LoadLibrary(L"ws2_32.dll");
        
        if (ws2_32 == nullptr) {
            log("Failed to get address of ws2_32.dll");
            return;
        }

        auto socketAddress = GetProcAddress(ws2_32, "socket");

        if (socketAddress == nullptr) {
            log("Failed to get address of ws2_32.socket");
            return;
        }

        g_disableNagle = this;
        m_hook = make_unique<FunctionHook>((uintptr_t)socketAddress, (uintptr_t)&DisableNagle::socket);

        if (m_hook->isValid()) {
            log("Successfully hooked ws2_32.socket");
        }
        else {
            log("Failed to hook ws2_32.socket");
        }

        log("Leaving DisableNagle constructor.");
    }

    DisableNagle::~DisableNagle() {
        g_disableNagle = nullptr;
    }

    void DisableNagle::onUI() {
        if (ImGui::CollapsingHeader("Disable Nagle")) {
            ImGui::Text("Disabling the nagle algorithm will reduce apparent lag to the server.");
            ImGui::Dummy(ImVec2{ 10.0f, 10.0f });
            ImGui::Checkbox("Enable Disable Nagle", &m_isEnabled);
        }
    }

    void DisableNagle::onConfigLoad(ConfigPtr cfg) {
        m_isEnabled = cfg->get_qualified_as<bool>("DisableNagle.Enabled").value_or(false);
    }

    void DisableNagle::onConfigSave(ConfigPtr cfg) {
        auto tbl = cpptoml::make_table();

        tbl->insert("Enabled", m_isEnabled);
        cfg->insert("DisableNagle", tbl);
    }

    SOCKET WSAAPI DisableNagle::socket(int af, int type, int protocol) {
        auto dn = g_disableNagle;

        auto originalSocket = (decltype(socket)*)dn->m_hook->getOriginal();
        auto result = originalSocket(af, type, protocol);

        if (dn->m_isEnabled) {
            int nodelay = 1;

            if (setsockopt(result, IPPROTO_TCP, TCP_NODELAY, (const char*)&nodelay, sizeof(nodelay)) == 0) {
                log("Nagle disabled successfully.");
            }
            else {
                log("Failed to disable nagle!");
            }
        }

        return result;
    }
}
