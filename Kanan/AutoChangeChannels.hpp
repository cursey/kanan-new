#pragma once

#include <FunctionHook.hpp>

#include "Mod.hpp"

namespace kanan {
class AutoChangeChannels : public Mod {
public:
    AutoChangeChannels();

    void onFrame() override;
    void onUI() override;

    void onConfigLoad(const Config& cfg) override;
    void onConfigSave(Config& cfg) override;

private:
    bool m_is_enabled{};
    int m_channel{1};
    std::unique_ptr<FunctionHook> m_add_condition_hook{};

    struct ConditionInfo {
        char pad[0x10];
        uint32_t id;
    };

    static int __fastcall hooked_add_condition(CCharacter::CConditionMgr::CCondition* condition, uint32_t EDX, int a2,
        char a3, int a4, int a5, ConditionInfo* info);
};
}
