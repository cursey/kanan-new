#pragma once

#include "Mod.hpp"

namespace kanan {
class CharacterWindowTitle : public Mod {
public:
    CharacterWindowTitle();

    void onFrame() override;
    void onUI() override;
    void onConfigLoad(const Config& cfg) override;
    void onConfigSave(Config& cfg) override;

private:
    bool m_is_enabled;
    std::string m_currentCharTitle;
};
} // namespace kanan