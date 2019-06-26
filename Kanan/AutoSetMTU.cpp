#include <algorithm>

#include <Windows.h>

#include <imgui.h>

#include <Scan.hpp>
#include <String.hpp>

#include "Log.hpp"
#include "AutoSetMTU.hpp"

using namespace std;

namespace kanan {
    static AutoSetMTU* g_autoSetMTU{ nullptr };

    AutoSetMTU::AutoSetMTU()
        : m_isEnabled{ false },
        m_interface{ "" },
        m_lowMTU{ 0 },
        m_normalMTU{ 0 },
        m_hook{}
    {
        log("Entering AutoSetMTU constructor.");
        log("Looking for connection address...");

        auto connectionAddress = scan("client.exe", "55 8B EC 83 EC ? 56 8B 75 0C 8B CE");

        if (connectionAddress) {
            log("Got connection address %p", *connectionAddress);

            g_autoSetMTU = this;
            m_hook = make_unique<FunctionHook>(*connectionAddress, (uintptr_t)&AutoSetMTU::createConnection);

            if (m_hook->isValid()) {
                log("Hooked the connection function.");
            }
            else {
                log("Failed to hook the connection function.");
            }

        }
        else{
            log("Failed to find connection address.");
        }

        log("Leaving AutoSetMTU constructor.");
    }

    void AutoSetMTU::onUI() {
        if (ImGui::CollapsingHeader("Auto Set MTU")) {
            ImGui::Text("This mod automatically sets your MTU when you login or change channels.");
            ImGui::Text("This can result in a more responsive connection with the game server, reducing apparent lag.");
            ImGui::Dummy(ImVec2{ 10.0f, 10.0f });
            ImGui::Text("To see what your interfaces are called, open a command prompt and type:");
            ImGui::Text("    netsh interface ipv4 show config");
            ImGui::Dummy(ImVec2{ 10.0f, 10.0f });
            ImGui::Checkbox("Enable Auto Set MTU", &m_isEnabled);
            ImGui::InputText("Interface", m_interface.data(), m_interface.size());
            ImGui::SliderInt("Lowered MTU", &m_lowMTU, 0, 1500);
            ImGui::SliderInt("Normal MTU", &m_normalMTU, 0, 1500);
        }
    }

    void AutoSetMTU::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("AutoSetMTU.Enabled").value_or(false);
        auto interface1 = cfg.get("AutoSetMTU.Interface").value_or("Ethernet");
        m_lowMTU = cfg.get<int>("AutoSetMTU.LoweredMTU").value_or(386);
        m_normalMTU = cfg.get<int>("AutoSetMTU.NormalMTU").value_or(1500);

        strcpy_s(m_interface.data(), m_interface.size(), interface1.c_str());
    }

    void AutoSetMTU::onConfigSave(Config& cfg) {
        cfg.set<bool>("AutoSetMTU.Enabled", m_isEnabled);
        cfg.set("AutoSetMTU.Interface", string{ m_interface.data() });
        cfg.set<int>("AutoSetMTU.LoweredMTU", m_lowMTU);
        cfg.set<int>("AutoSetMTU.NormalMTU", m_normalMTU);
    }

    optional<DWORD> AutoSetMTU::runProcess(const string& name, const string& params) {
        auto commandLine = widen(name + " " + params);
        STARTUPINFO startupInfo{};
        PROCESS_INFORMATION processInfo{};

        startupInfo.cb = sizeof(startupInfo);

        if (CreateProcess(nullptr, (LPWSTR)commandLine.c_str(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW | NORMAL_PRIORITY_CLASS, nullptr, nullptr, &startupInfo, &processInfo) == FALSE) {
            log("Failed to CreateProcess for %s %s", name.c_str(), params.c_str());
            return {};
        }

        auto processHandle = processInfo.hProcess;
        DWORD exitCode{ 0 };

        if (WaitForSingleObject(processHandle, 5000) != WAIT_OBJECT_0) {
            log("Failed to wait for %s %s", name.c_str(), params.c_str());
            return {};
        }

        if (GetExitCodeProcess(processHandle, &exitCode) == FALSE) {
            log("Failed to get the exit code for %s %s", name.c_str(), params.c_str());
            return {};
        }

        return exitCode;
    }

    char AutoSetMTU::createConnection(int a1, int a2) {
        auto mtu = g_autoSetMTU;

        // Lower the MTU if we are enabled.
        if (mtu->m_isEnabled) {
            auto lowMTU = to_string(mtu->m_lowMTU);
            auto interface1 = string{ mtu->m_interface.data() };

            if (mtu->runProcess("netsh.exe", "interface ipv4 set subinterface \"" + interface1 + "\" mtu=" + lowMTU + " store=persistent")) {
                log("Lowered MTU successfully.");
            }
        }

        // Call the original connection function to actually create the connection.
        auto originalConnection = (decltype(createConnection)*)mtu->m_hook->getOriginal();
        auto result = originalConnection(a1, a2);

        // Restore the MTU to normal now that the connection has been created.
        if (mtu->m_isEnabled) {
            auto normalMTU = to_string(mtu->m_normalMTU);
            auto interface1 = string{ mtu->m_interface.data() };

            if (mtu->runProcess("netsh.exe", "interface ipv4 set subinterface \"" + interface1 + "\" mtu=" + normalMTU + " store=persistent")) {
                log("Restored MTU successfully.");
            }
        }

        return result;
    }
}
