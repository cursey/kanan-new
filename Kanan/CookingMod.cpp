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

    void CookingMod::HookForCooking(int* _this, int ingredientIdx, int amount) {
        if (!g_cooking->m_is_enabled) {
            g_cooking->m_hook->callOriginal(_this, ingredientIdx, amount);
            return;
        }
        g_cooking->m_hook->callOriginal(_this, 0, cookingOne);
        g_cooking->m_hook->callOriginal(_this, 1, cookingTwo);
        g_cooking->m_hook->callOriginal(_this, 2, cookingThree);

        //work around if you add some ingredient before enabled cooking mod, it may mess up amounts.
        _this[188] = cookingOne;
        _this[189] = cookingTwo;
        _this[190] = cookingThree;
    }  

    //find the addresses we need for patching
    CookingMod::CookingMod():m_hook(nullptr) {
        auto cookingAddress = scan("client.exe", "40 57 48 83 EC 30 48 C7 44 24 20 FE FF FF FF 48 89 5C 24 40 48 89 74 24 48 48 8B F1 44 8B 89 F8 02 00 00 44 03 89 F4 02 00 00 44 03 89 F0 02 00 00 41 81 F9 40 42 0F 00 ");
       
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
    }

    void CookingMod::onConfigSave(Config& cfg) {
        cfg.set<bool>("Cooking.Enabled", m_is_enabled);
    } 
    
}
