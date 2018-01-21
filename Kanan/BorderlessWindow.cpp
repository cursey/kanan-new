#include <thread>

#include <imgui.h>

#include "Log.hpp"
#include "Kanan.hpp"
#include "BorderlessWindow.hpp"

using namespace std;

namespace kanan {
    BorderlessWindow::BorderlessWindow()
        : m_defaultStyle{ 0 },
        m_style{ 0 },
        m_x{ 0 }, m_y{ 0 }, m_w{ 0 }, m_h{ 0 },
        m_changeStyle{ false },
        m_changePos{ false },
        m_choice{ 0 },
        m_isChoiceFulfilled{ false }
    {
    }

    // NOTE: Mabinogi is stubborn about its window position and style when it first
    // starts up so we try setting the requested style multiple times over multiple
    // frames in the onFrame callback instead of just doing it once in apply().
    void BorderlessWindow::onFrame() {
        if (m_isChoiceFulfilled) {
            return;
        }

        auto wnd = g_kanan->getWindow();

        // Set the style.
        if (m_changeStyle && SetWindowLong(wnd, GWL_STYLE, m_style) != m_style) {
            return;
        }

        // Move the window.
        if (m_changePos && SetWindowPos(wnd, HWND_TOP, m_x, m_y, m_w, m_h, SWP_FRAMECHANGED) == FALSE) {
            return;
        }

        m_isChoiceFulfilled = true;
    }

    void BorderlessWindow::onUI() {
        if (ImGui::CollapsingHeader("Borderless Window")) {
            ImGui::Combo("Style", &m_choice, "Disabled\0Borderless Window\0Borderless Fullscreen\0\0");

            if (ImGui::Button("Apply")) {
                apply();
            }
        }
    }

    void BorderlessWindow::onConfigLoad(const Config& cfg) {
        m_choice = cfg.get<int>("BorderlessWindow.Choice").value_or(0);

        if (m_choice != 0) {
            apply();
        }
    }

    void BorderlessWindow::onConfigSave(Config& cfg) {
        cfg.set<int>("BorderlessWindow.Choice", m_choice);
    }

    void BorderlessWindow::apply() {
        log("Applying Borderless Window setting...");

        // Get the window we want to change the style of.
        auto wnd = g_kanan->getWindow();

        if (wnd == nullptr) {
            return;
        }

        // If we haven't recorded what the default style of the window is, then
        // get it.
        if (m_defaultStyle == 0) {
            m_defaultStyle = GetWindowLong(wnd, GWL_STYLE);
        }

        m_style = m_defaultStyle;

        switch (m_choice) {
        case 0:
            m_changeStyle = true;
            m_changePos = false;
            break;

        case 1: {
            RECT r{};

            SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);

            m_x = r.left;
            m_y = r.top;
            m_w = r.right - r.left;
            m_h = r.bottom - r.top;
            m_style &= ~(WS_BORDER | WS_CAPTION | WS_THICKFRAME);

            m_changeStyle = true;
            m_changePos = true;

            break;
        }

        case 2:
            m_w = GetSystemMetrics(SM_CXSCREEN);
            m_h = GetSystemMetrics(SM_CYSCREEN);
            m_style &= ~(WS_BORDER | WS_CAPTION | WS_THICKFRAME);

            m_changeStyle = true;
            m_changePos = true;

            break;

        default:
            break;
        }

        // At this point the style choice hasn't been fulfilled.
        m_isChoiceFulfilled = false;
    }
}
