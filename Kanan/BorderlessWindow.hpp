#pragma once

#include <Windows.h>

#include "Mod.hpp"

namespace kanan {
    class BorderlessWindow : public Mod {
    public:
        BorderlessWindow();

        void onFrame() override;
        void onUI() override;

        void onConfigLoad(ConfigPtr cfg) override;
        void onConfigSave(ConfigPtr cfg) override;

    private:
        LONG m_defaultStyle;
        LONG m_style;
        int m_x, m_y, m_w, m_h;

        int m_choice;
        int m_setStyleCounter;

        void apply();
    };
}
