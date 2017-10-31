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
        m_choice{ 0 },
        m_setStyleCounter{ 0 }
    {
    }

    // NOTE: Mabinogi is stubborn about its window position and style when it first
    // starts up so we try setting the requested style multiple times over multiple
    // frames in the onFrame callback instead of just doing it once in apply().
    void BorderlessWindow::onFrame() {
        if (m_choice == 0 || m_setStyleCounter > 10) {
            return;
        }

        auto wnd = g_kanan->getWindow();

        // Set the style.
        SetWindowLong(wnd, GWL_STYLE, m_style);

        // Move the window.
        SetWindowPos(wnd, HWND_TOP, m_x, m_y, m_w, m_h, SWP_FRAMECHANGED);

        ++m_setStyleCounter;
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

        // If the choice is 0 just restore the default style.
        if (m_choice == 0) {
            SetWindowLong(wnd, GWL_STYLE, m_defaultStyle);
            return;
        }

        m_style = m_defaultStyle;

        switch (m_choice) {
        case 1: {
            RECT r{};

            SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);

            m_x = r.left;
            m_y = r.top;
            m_w = r.right - r.left;
            m_h = r.bottom - r.top;
            m_style &= ~(WS_BORDER | WS_CAPTION | WS_THICKFRAME);

            break;
        }

        case 2:
            m_w = GetSystemMetrics(SM_CXSCREEN);
            m_h = GetSystemMetrics(SM_CYSCREEN);
            m_style &= ~(WS_BORDER | WS_CAPTION | WS_THICKFRAME);

            break;

        default:
            break;
        }

        // Reset the counter so that onFrame attempts to set the desired styling.
        m_setStyleCounter = 0;
    }
}
