#include "CookingMod.hpp"
#include "Log.hpp"

#include <SafetyHook.hpp>


namespace kanan {
    // just a temp vector for the imgui ui
    int cookingTemp[3] = { 0, 0, 0 };

     

    //write a call to the custom code eg the hook
    //find the addresses we need for patching
    CookingMod::CookingMod() {
        auto cookingAddress = scan("client.exe", "44 01 ? ? ? ? ? ? E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 C7 44 24 58 ? ? ? ? 48 8D ? ? ? ? ? E8 ? ? ? ? 8B F8 8D 48 ? E8 ? ? ? ? 48 89 ? ? ? 48 8D ? ? 4C 8D ? ? ? ? ? 8D 57 ? E8 ? ? ? ? 48 8B ? ? ? 89 79 ? 48 8B ? ? ? ? ? E8 ? ? ? ? 48 8B ? E8 ? ? ? ? 48 8B ? 41 B0 ? 48 8D ? ? ? E8 ? ? ? ? 90 48 8B ? ? ? 48 85 ? 74 ? E8 ? ? ? ? 48 C7 44 24 58 ? ? ? ? 41 B8 ? ? ? ? 48 8D ? ? ? ? ? 48 8B ? ? ? ? ? E8 ? ? ? ? 48 8B ? ? ? ? ? 48 8B ? B2 ? FF 90 ? ? ? ? 48 8B ? ? ? 48 8B ? ? ? 48 83 C4 ? 5F C3 CC CC CC CC CC CC CC 40");
        if (!cookingAddress) {
            log("[Cooking]unable to find cooking address");
        }
        else {
            log("[Cooking]found cooking address base @ %p", *cookingAddress);
        }

    }


    //imgui ui stuff
    void CookingMod::onUI() {
        if (ImGui::CollapsingHeader("Cooking mod"))
        {
            if (ImGui::Checkbox("Enable Cooking mod", &m_is_enabled)) {
                CookingMod::applycook(m_is_enabled);
            }
            if (m_is_enabled) {

                ImGui::TextWrapped("Cooking %");
                ImGui::InputInt3("", (int*)&cookingTemp);
                cookingOne = cookingTemp[0] * 10000;
                cookingTwo = cookingTemp[1] * 10000;
                cookingThree = cookingTemp[2] * 10000;

            }
        }

    }

    //apply or remove our patch
    void CookingMod::applycook(bool m_cooking_is_enabled) {
        if (m_cooking_is_enabled) {
            log("[Cooking]Cooking applying patch");
            //inject our custom cooking asm
            //todo: add patch
        }
        else if (!m_cooking_is_enabled) {
            log("[Cooking]Cooking removing patch");
            //todo: reverse patch
        }
    }

    //config stuff
    void CookingMod::onConfigLoad(const Config& cfg) {
        m_is_enabled = cfg.get<bool>("Cooking.Enabled").value_or(false);
        if (m_is_enabled) {
            CookingMod::applycook(m_is_enabled);
        }
    }
    void CookingMod::onConfigSave(Config& cfg) {
        cfg.set<bool>("Cooking.Enabled", m_is_enabled);
    }


}