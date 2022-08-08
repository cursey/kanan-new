#include "CookingMod.hpp"
#include "Kanan.hpp"
#include <Scan.hpp>
#include <Utility.hpp>
#include <imgui.h>
#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <Utility.hpp>
#include <iostream>

#include "Log.hpp"


namespace kanan {
    //our cooking vaules we will set
    int cookingOne = 0;
    int cookingTwo = 0;
    int cookingThree = 0;

    // just a temp vector for the imgui ui
    int cookingTemp[3] = { 0, 0, 0 };


    //cooking address
    DWORD cookingAddressx;
    DWORD addressOfCooking;



    //the asm we want to inject
    void __declspec(naked) HookForCooking()
    {
        __asm
        {
            mov eax, 00000000
            mov esi, [cookingOne]
            mov[edi + eax * 4 + 0x0000020C], esi
            mov eax, 00000001
            mov esi, [cookingTwo]
            mov[edi + eax * 4 + 0x0000020C], esi
            mov eax, 00000002
            mov esi, [cookingThree]
            mov[edi + eax * 4 + 0x0000020C], esi
            ret

        }

    }


    //the code i use for injecting stuff
    //patch bytes
    void Patchmem(BYTE* dst, BYTE* src, unsigned int size)
    {
        DWORD oldprotect;
        //set prems of area of memory to execute&read&write, copy the old perms into oldprotect
        VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
        //write our new bytes into the dst with the bytes in src
        memcpy(dst, src, size);
        //set the perms of the area of memory back to what ever it was before we were here
        VirtualProtect(dst, size, oldprotect, &oldprotect);

    }


    //write a call to the custom code eg the hook
    bool Hookcall(void* toHook, void* ourFunct, int len)
    {
        if (len < 5)
        {
            return false;
        }
        DWORD curProtection;
        VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);
        memset(toHook, 0x90, len);

        DWORD relativeAddress = ((DWORD)ourFunct - (DWORD)toHook) - 5;
        *(BYTE*)toHook = 0xE8;
        *(DWORD*)((DWORD)toHook + 1) = relativeAddress;

        DWORD temp;
        VirtualProtect(toHook, len, curProtection, &temp);

        return true;
    }


    //find the addresses we need for patching
    CookingMod::CookingMod() {
        auto cookingAddress = scan("client.exe", "55 8B EC 6A ? 68 ? ? ? ? 64 ? ? ? ? ? ? ? ? A1 ? ? ? ? 33 C5 50 8D ? F4 64 ? ? ? ? ? ? ? 8B 97 14 ? 00 00");
        DWORD cookingAddresst = *cookingAddress;
        if (!cookingAddresst) {
            log("unable to find cooking address");
        }
        else {
            log("found cooking address base @ %p", cookingAddresst);
        }

        if (cookingAddresst) {

            addressOfCooking = cookingAddresst + 95;
            log("Address of func to change @ %p", addressOfCooking);

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
    //apply or remove our patch
    void CookingMod::applycook(bool m_cooking_is_enabled) {
        if (m_cooking_is_enabled) {
            log("Cooking applying patch");
            //inject our custom cooking asm
            Hookcall((void*)addressOfCooking, HookForCooking, 7);

        }
        else if (!m_cooking_is_enabled) {
            log("Cooking removing patch");
            //patch back to default
            Patchmem((BYTE*)(addressOfCooking), (BYTE*)"\x01\xb4\x87\x0C\x02\x00\x00", 7);
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