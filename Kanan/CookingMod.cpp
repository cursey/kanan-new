#include "CookingMod.hpp"
#include "Kanan.hpp"
#include <Scan.hpp>
#include <imgui.h>
#include "Log.hpp"


namespace kanan {
    //our cooking vaules we will set
    int cookingOne = 0;
    int cookingTwo = 0;
    int cookingThree = 0;

    // just a temp vector for the imgui ui
    int cookingTemp[3] = { 0, 0, 0 };

    static CookingMod* g_cooking;

    //variable names may not accurately reflect the true names of the class. The original hooked function (see kanan pull request # 278)
    //had args for _this, ingredientIdx and Amount. Ingredient Idx is now part of the CookingViewClass (0x2FC). _this is a pointer to CCookingView2 
    void CookingMod::HookForCooking(int* _this, int amount) {
        if (!g_cooking->m_is_enabled) {
            g_cooking->m_hook->callOriginal(_this, amount);
            return;
        }
        auto ingredientIdx = _this[193]; //Interface button we are currently clicking
        if (ingredientIdx > -1 && ingredientIdx < 3) { //-1 is a state of not clicking any button. 0 through 2 are each Add button starting from the left
            //Manually set each food percent of the object from our kanan UI
                _this[188] = cookingOne;//+2F0
                _this[189] = cookingTwo;
                _this[190] = cookingThree;

                //subtract 1 from the button we are pressing to allow the the original function to set the cooking bar full flag correctly
                //If we don't do this the code that handles activating the start button does not get executed
                _this[188 + ingredientIdx] = _this[188 + ingredientIdx] - 1;
                //we call the original function - effectively adding the 1 value we just subtracted and allowing the start button to appear.
                g_cooking->m_hook->callOriginal(_this, amount);
                return;
        } else {
            g_cooking->m_hook->callOriginal(_this, amount);
            return;
        }
    }  

    //find the addresses we need for patching
    CookingMod::CookingMod():m_hook(nullptr) {
    auto cookingAddress = scan("client.exe", "48 89 ? ? ? 48 89 ? ? ? 56 57 41 ? 48 83 EC ? 0F 29 ? ? ? 44 8B ? 48 8B ? 45 33 ? 0F B6 ? ? ? ? ? E8 ? ? ? ? 44 8B ? ? ? ? ? 41 83 F9 ? 0F 84 ? ? ? ? 41 83 F9 ? 0F 8D ? ? ? ? 8B 85 ? ? ? ? 85 C0 0F 84 ? ? ? ? 44 3B ? 0F 86 ? ? ? ? 41 8B ? 2B C8 0F AF ? ? ? ? ? B8 ? ? ? ? F7 E1 C1 EA ? 44 8B ? ? ? ? ? 44 03 ? ? ? ? ? 44 03 ? ? ? ? ? 41 81 F8 ? ? ? ? 0F 83 ? ? ? ? 41 8D ? ? B9 ? ? ? ? 41 2B ? 3D ? ? ? ? 0F 46 ? 85 C9 0F 84 ? ? ? ? 42 01 ? ? ? ? ? ? 48 8B ? E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B ? ? ? ? ? E8 ? ? ? ? 48 8B ? E8 ? ? ? ? 48 8B ? 48 C7 44 24 70 ? ? ? ? 48 8D ? ? ? ? ? E8 ? ? ? ? 8B F8 8D 48 ? E8 ? ? ? ? 48 89 ? ? ? 48 8D ? ? 4C 8D ? ? ? ? ? 8D 57 ? E8 ? ? ? ? 48 8B ? ? ? 89 79 ? 41 B0 ? 48 8D ? ? ? 48 8B ? E8 ? ? ? ? 90 48 8B ? ? ? 48 85 ? 74 ? E8 ? ? ? ? 48 C7 44 24 70 ? ? ? ? 41 B8 ? ? ? ? 48 8D ? ? ? ? ? 48 8B ? ? ? ? ? E8 ? ? ? ? 48 8B ? ? ? ? ? 48 8B ? B2 ? FF 90 ? ? ? ? 44 89 ? ? ? ? ? 48 8B ? ? ? ? ? 48 8B ? ? ? ? ? E8 ? ? ? ? 8B BD ? ? ? ? 03 BD ? ? ? ? 03 BD ? ? ? ? 48 85 ? 0F 84 ? ? ? ? 8D 4F ? 81 F9 ? ? ? ? 0F 87 ? ? ? ? 48 8B ? ? ? ? ? 48 8B ? E8 ? ? ? ? 48 8B ? E8 ? ? ? ? 0F 28 ? 48 8B ? E8 ? ? ? ? 0F 28 ? F3 0F ? ? ? ? ? ? 48 8B");
        if (!cookingAddress) {
            log("unable to find cooking address");
        }
        else {
            log("found cooking address base @ %p", *cookingAddress);
            m_hook = std::make_unique<FunctionHook>(*cookingAddress, reinterpret_cast<uintptr_t>(HookForCooking));
        }
        g_cooking = this;
    }

    //imgui ui stuff
    void CookingMod::onUI() {
        if (ImGui::CollapsingHeader("Cooking mod"))
        {
            ImGui::Checkbox("Enable Cooking mod", &m_is_enabled);
            if (m_is_enabled) {

                ImGui::TextWrapped("Cooking %");
                ImGui::InputInt3("", (int*)&cookingTemp);
                CookingMod::cookingMath(cookingTemp);
            }
        }
    }

    //assign the vaules to the correct places
    void CookingMod::cookingMath(int cookingTemp[3]) {
        cookingOne = cookingTemp[0] * 10000;
        cookingTwo = cookingTemp[1] * 10000;
        cookingThree = cookingTemp[2] * 10000;
    }    

    //config stuff
    void CookingMod::onConfigLoad(const Config& cfg) {
        m_is_enabled = cfg.get<bool>("Cooking.Enabled").value_or(false);
        cookingTemp[0] = cfg.get<int>("Cooking.cookingOne").value_or(0);
        cookingTemp[1] = cfg.get<int>("Cooking.cookingTwo").value_or(0);  
        cookingTemp[2] = cfg.get<int>("Cooking.cookingThree").value_or(0);  
    }

    void CookingMod::onConfigSave(Config& cfg) {
        cfg.set<bool>("Cooking.Enabled", m_is_enabled);
        cfg.set<int>("Cooking.cookingOne", cookingTemp[0]);
        cfg.set<int>("Cooking.cookingTwo", cookingTemp[1]);
        cfg.set<int>("Cooking.cookingThree", cookingTemp[2]);
    } 
    
}
