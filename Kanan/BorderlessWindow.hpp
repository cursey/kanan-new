#pragma once

#include <Windows.h>

#include "Mod.hpp"

namespace kanan {
    class BorderlessWindow : public Mod {
    public:
        struct Monitor {
            std::string name;
            RECT rect;
            RECT workRect;
        };

        BorderlessWindow();

        void onFrame() override;
        void onUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

        bool onMessage(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) override;

        auto& getMonitors() {
            return m_monitors;
        }

    private:
        LONG m_defaultStyle;
        LONG m_style;
        int m_x, m_y, m_w, m_h;
        bool m_changeStyle;
        bool m_changePos;

        int m_styleChoice;
        bool m_isChoiceFulfilled;

        std::vector<Monitor> m_monitors;
        int m_monitorChoice;

        bool m_dontSetStyle;

        void apply();
    };
}
