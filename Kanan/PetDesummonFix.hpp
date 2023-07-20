#pragma once

#include <Mod.hpp>
#include <imgui.h>

namespace kanan {

class PetDesummonFix : public PatchMod {
public:
    PetDesummonFix();

    void onPatchUI() override;

    void onConfigLoad(const Config& cfg) override;
    void onConfigSave(Config& cfg) override;

private:
    bool m_isEnabled{};
    std::unique_ptr<FunctionHook> m_petHotkeyHook;
    HANDLE* m_petHotkeyHookParam1;
    static void __fastcall PetHotkeyHook(LONGLONG param1, int param2);
};
} // namespace kanan
#pragma once
