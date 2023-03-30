#pragma once

#include "Mod.hpp"

namespace kanan {
class DrawDistance : public Mod {
public:
    DrawDistance();

    void onFrame() override;

    void onUI() override;

    void onConfigLoad(const Config& cfg) override;
    void onConfigSave(Config& cfg) override;

private:
    float m_distance;
    bool m_isEnabled;
};
} // namespace kanan
