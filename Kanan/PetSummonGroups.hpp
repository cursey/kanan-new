#pragma once

#include <Mod.hpp>
#include <imgui.h>

namespace kanan {
class PetSummonGroups : public Mod {
public:
    PetSummonGroups();

    void onUI() override;

    void onConfigLoad(const Config& cfg) override;
    void onConfigSave(Config& cfg) override;

private:
    bool m_isEnabled{};
    std::unique_ptr<FunctionHook> m_hook;
    static void __fastcall HookForSummoning(int hotkeyPressed, int tabFlag);
};
} // namespace kanan
#pragma once
