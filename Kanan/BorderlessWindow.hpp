#pragma once

#include <Windows.h>

#include "Mod.hpp"

namespace kanan {
    class BorderlessWindow : public Mod {
    public:
        BorderlessWindow();

        void onFrame() override;
        void onUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        LONG m_defaultStyle;
        LONG m_style;
        int m_x, m_y, m_w, m_h;
        bool m_changeStyle;
        bool m_changePos;

        int m_choice;
        bool m_isChoiceFulfilled;

        void apply();
    };
}
