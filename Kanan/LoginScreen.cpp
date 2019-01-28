#include <imgui.h>

#include <Scan.hpp>
#include <Patch.hpp>
#include <Utility.hpp>
#include <String.hpp>
#include <Module.hpp>

#include "Log.hpp"
#include "LoginScreen.hpp"


using namespace std;

namespace kanan {
    LoginScreen::LoginScreen()
        : m_isEnabled{ false },
        m_isApplied{ false },
        m_loginScenes{},
        m_choice{ 0 },
        m_comboStr{""},
        m_isDebug{ false }
    {
        log("[LoginScreen] Entering constructor...");
        HMODULE mod = GetModuleHandle(widen("client.exe").c_str());
        auto size = getModuleSize(mod).value_or(0);
        char *pat1 = ".?AVCLoginScene@pleione";
        auto loginSceneRTIIBase = scan((uintptr_t)mod + 0x2C00000, size - 0x2C00000, hexify((const uint8_t*)pat1, strnlen_s(pat1, 100)));
        if (loginSceneRTIIBase)
        {
            if (m_isDebug) log("[LoginScreen] Found address of .?AVCLoginScene@pleione %p", loginSceneRTIIBase);
        }
        else {
            log("[LoginScreen] Failed to get address of .?AVCLoginScene@pleione");
            goto fail;
        }
        auto rttiStrAddr = *loginSceneRTIIBase + 1;
        auto RTTICompleteObjectLocator = (uintptr_t)mod + 0x2C00000;
        auto VFTableAddr = (uintptr_t)mod + 0x2C00000;
        auto constructorRefAddr = (uintptr_t)mod;
        while (1)
        {
            char *pat2 = ".?AVCLoginScene";
            auto rttiStrAddrP = scan(rttiStrAddr, size - (rttiStrAddr - (uintptr_t)mod), hexify((const uint8_t*)pat2, strnlen_s(pat2, 100)));
            if (!rttiStrAddrP) break;
            rttiStrAddr = *rttiStrAddrP;
            std::string rttiStr((char*)rttiStrAddr + 5);
            rttiStr = rttiStr.substr(0, rttiStr.find('@'));
            if (m_isDebug) log("[LoginScreen] %s", rttiStr.c_str());
            if (rttiStr.find("XmlLoad") == string::npos)
            {
                auto RTTITypeDescriptor = rttiStrAddr - 8;
                auto RTTICompleteObjectLocatorP = scan(RTTICompleteObjectLocator, size - (RTTICompleteObjectLocator - (uintptr_t)mod), hexify((const uint8_t*)&RTTITypeDescriptor, 4));
                if (RTTICompleteObjectLocatorP)
                {
                    if (m_isDebug) log("[LoginScreen] Found address of RTTICompleteObjectLocator %p", RTTICompleteObjectLocatorP);
                    RTTICompleteObjectLocator = *RTTICompleteObjectLocatorP - 4 * 3;
                    auto VFTableAddrP = scan(VFTableAddr, size - (VFTableAddr - (uintptr_t)mod), hexify((const uint8_t*)&RTTICompleteObjectLocator, 4));
                    if (VFTableAddrP)
                    {
                        if (m_isDebug) log("[LoginScreen] Found address of VFTableAddr %p", VFTableAddrP);
                        VFTableAddr = *VFTableAddrP + 4;
                        auto constructorRefAddrP = scan(constructorRefAddr, size - (constructorRefAddr - (uintptr_t)mod), hexify((const uint8_t*)&VFTableAddr, 4));
                        if (constructorRefAddrP)
                        {
                            constructorRefAddr = *constructorRefAddrP;
                            if (m_isDebug) log("[LoginScreen] Found address of constructorRefAddr %p", constructorRefAddrP);
                            auto constructorAddr = scan(*constructorRefAddrP - 0x50, 0x50, "55 8B EC 6A FF");
                            if (constructorAddr)
                            {
                                if (m_isDebug) log("[LoginScreen] Found address of constructorAddr %p", constructorAddr);
                                m_loginScenes.push_back(LoginSceneElement(rttiStr, *constructorAddr));
                            }
                        }
                        VFTableAddr += 5;
                    }
                    RTTICompleteObjectLocator += 4 * 3 + 1;
                }
            }
            rttiStrAddr++;
        }
        for (auto scene : m_loginScenes)
            m_comboStr += scene.m_name + "$";
        m_comboStr += "$";
        for (size_t i = 0; i < m_comboStr.length(); ++i)
            if (m_comboStr.c_str()[i] == '$') ((char*)&m_comboStr.at(0))[i] = 0; // I'm terrible person.
        fail:
        log("[LoginScreen] Leaving constructor.");
    }

    void LoginScreen::onUI() {
        if (ImGui::CollapsingHeader("Login Screen")) {
            ImGui::TextWrapped(
                "Change Login Screen"
                "NOTE: You must restart the game after changing settings for this mod."
            );
            ImGui::Spacing();

            ImGui::Checkbox("Change Login Screen", &m_isEnabled);

            if (ImGui::Combo("Login Scene", &m_choice, m_comboStr.c_str())) {
                m_sceneName = m_loginScenes[m_choice].m_name;
            }
        }
    }

    void LoginScreen::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("LoginScreen.Enabled").value_or(false);

        m_sceneName = cfg.get("LoginScreen.SceneName").value_or("");

        for (size_t i = 0; i < m_loginScenes.size(); ++i)
        {
            if (m_loginScenes[i].m_name == m_sceneName)
                m_choice = i;
        }

        if (m_isEnabled) {
            apply();
        }
    }

    void LoginScreen::onConfigSave(Config& cfg) {
        cfg.set<bool>("LoginScreen.Enabled", m_isEnabled);
        cfg.set("LoginScreen.SceneName", m_sceneName);
    }

    void LoginScreen::apply()
    {
        if (m_isApplied) return;
        m_isApplied = true;
        uintptr_t constructorAddr = 0;

        for(auto scene : m_loginScenes)
        {
            if (scene.m_name == m_sceneName)
                constructorAddr = scene.m_constructorAddr;
        }
        if (constructorAddr == 0) return;

        kanan::Patch loginPatch;
        // Patch jump to old login scenes
        auto loginScreenPatch1 = scan("client.exe", "E8 ? ? ? ? 84 C0 74 33 68 ? ? ? ? E8 ? ? ? ? 83 C4 04 89 45 0C C6 45 FC 36 85 C0 74 10 8B C8 E8 ? ? ? ? C6 45 FC 05");
        if (loginScreenPatch1)
        {
            if (m_isDebug) log("[LoginScreen] loginScreenPatch1: %p", loginScreenPatch1);
        }
        else
        {
            log("[LoginScreen] loginScreenPatch1: fail");
            return;
        }
        loginPatch.address = *loginScreenPatch1;
        loginPatch.bytes = { 0x90, 0x90, 0x90, 0x90, 0x90 };
        kanan::patch(loginPatch);
        loginPatch.address = *loginScreenPatch1 + 7;
        loginPatch.bytes = { 0xEB };
        kanan::patch(loginPatch);

        // Patch jumps to call first login scene constructor
        auto loginScreenPatch2 = scan("client.exe", "68 ? ? ? ? 8B F8 8B F2 E8 ? ? ? ? 84 C0 74 2B 68");
        if (loginScreenPatch2)
        {
            if (m_isDebug) log("[LoginScreen] loginScreenPatch2: %p", loginScreenPatch2);
        }
        else
        {
            log("[LoginScreen] loginScreenPatch2: fail");
            return;
        }
        loginPatch.address = *loginScreenPatch2 + 16;
        loginPatch.bytes = { 0xEB };
        kanan::patch(loginPatch);

        auto loginScreenPatch3 = scan("client.exe", "68 78 04 00 00 E8 ? ? ? ? 84 C0 74 2B 68 80 00 00 00 E8 ? ? ? ? 83 C4 04 89 45 F0 C7 45 ? ? ? ? ? 85 C0 0F 84 ? ? ? ? 8B C8 E8");
        if (loginScreenPatch3)
        {
            if (m_isDebug) log("[LoginScreen] loginScreenPatch3: %p", loginScreenPatch3);
        }
        else
        {
            log("[LoginScreen] loginScreenPatch3: fail");
            return;
        }
        loginPatch.address = *loginScreenPatch3 + 12;
        loginPatch.bytes = { 0x90, 0x90 };
        loginPatch.address = *loginScreenPatch3 + 16; // increase new() size
        loginPatch.bytes = { 0x10 };
        kanan::patch(loginPatch);
        // patch login scene constructor call
        auto callInstructionAddr = *loginScreenPatch3 + 47;
        *(uintptr_t*)(callInstructionAddr + 1) = (constructorAddr - 5) - callInstructionAddr;
    }

}