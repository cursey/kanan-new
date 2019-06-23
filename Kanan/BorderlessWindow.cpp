#include <thread>

#include <imgui.h>

#include <String.hpp>

#include "Log.hpp"
#include "Kanan.hpp"
#include "BorderlessWindow.hpp"

using namespace std;

namespace kanan {
    BOOL CALLBACK onMonitor(HMONITOR monitor, HDC hdc, LPRECT rect, LPARAM data) {
        UNREFERENCED_PARAMETER(hdc);
        UNREFERENCED_PARAMETER(rect);

        auto me = (BorderlessWindow*)data;

        MONITORINFOEX monitorInfo{};

        monitorInfo.cbSize = sizeof(MONITORINFOEX);

        if (GetMonitorInfo(monitor, &monitorInfo) == FALSE) {
            return TRUE;
        }

        BorderlessWindow::Monitor bwMonitor{};

        bwMonitor.name = narrow(monitorInfo.szDevice);
        bwMonitor.rect = monitorInfo.rcMonitor;
        bwMonitor.workRect = monitorInfo.rcWork;

        me->getMonitors().push_back(bwMonitor);

        return TRUE;
    }

    BorderlessWindow::BorderlessWindow()
        : m_defaultStyle{ 0 },
        m_style{ 0 },
        m_x{ 0 }, m_y{ 0 }, m_w{ 0 }, m_h{ 0 },
        m_changeStyle{ false },
        m_changePos{ false },
        m_styleChoice{ 0 },
        m_isChoiceFulfilled{ false },
        m_monitors{},
        m_monitorChoice{ 0 }
    {
        EnumDisplayMonitors(nullptr, nullptr, onMonitor, (LPARAM)this);
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
        if (m_changeStyle) {
            log("[BorderlessWindow] Trying to set style: %ld", m_style);

            const auto isStyleSet = SetWindowLong(wnd, GWL_STYLE, m_style) == m_style;
            const auto isStylePosSet = SetWindowPos(wnd, HWND_TOP, m_x, m_y, m_w, m_h, SWP_FRAMECHANGED) != FALSE;

            if (!isStyleSet || !isStylePosSet) {
                return;
            }

            log("[BorderlessWindow] Style set!");

            m_changeStyle = false;
            return;
        }

        // Move the window.
        if (m_changePos) { 
            log("[BorderlessWindow] Trying to move to x:%d y:%d w:%d h:%d", m_x, m_y, m_w, m_h);

            const auto wasWindowMoved = MoveWindow(wnd, m_x, m_y, m_w, m_h, TRUE) != FALSE;

            if (!wasWindowMoved) {
                return;
            }

            log("[BorderlessWindow] Window moved!");

            m_changePos = false;
            return;
        }

        m_isChoiceFulfilled = true;

        log("[BorderlessWindow] Choices applied.");
    }

    void BorderlessWindow::onUI() {
        if (ImGui::CollapsingHeader("Borderless Window")) {
            ImGui::Combo("Style", &m_styleChoice, "Disabled\0Borderless Window\0Borderless Fullscreen\0Custom\0\0");

            if (m_styleChoice == 3) {
                ImGui::InputInt2("Position", &m_x);
                ImGui::InputInt2("Size", &m_w);
            }

            string monitorChoices{};

            for (auto& monitor : m_monitors) {
                monitorChoices += monitor.name + '\0';
            }

            if (!monitorChoices.empty()) {
                monitorChoices += '\0';

                ImGui::Combo("Monitor", &m_monitorChoice, monitorChoices.c_str());
            }

            if (ImGui::Button("Apply")) {
                apply();
            }
        }
    }

    void BorderlessWindow::onConfigLoad(const Config& cfg) {
        m_styleChoice = cfg.get<int>("BorderlessWindow.Choice").value_or(0);
        m_monitorChoice = cfg.get<int>("BorderlessWindow.Monitor").value_or(0);
        m_x = cfg.get<int>("BorderlessWindow.X").value_or(0);
        m_y = cfg.get<int>("BorderlessWindow.Y").value_or(0);
        m_w = cfg.get<int>("BorderlessWindow.W").value_or(1920);
        m_h = cfg.get<int>("BorderlessWindow.H").value_or(1080);

        if (m_styleChoice != 0) {
            apply();
        }
    }

    void BorderlessWindow::onConfigSave(Config& cfg) {
        cfg.set<int>("BorderlessWindow.Choice", m_styleChoice);
        cfg.set<int>("BorderlessWindow.Monitor", m_monitorChoice);
        cfg.set<int>("BorderlessWindow.X", m_x);
        cfg.set<int>("BorderlessWindow.Y", m_y);
        cfg.set<int>("BorderlessWindow.W", m_w);
        cfg.set<int>("BorderlessWindow.H", m_h);
    }

    bool BorderlessWindow::onMessage(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (m_styleChoice == 0) {
            return true;
        }

        switch (message) {
        case WM_WINDOWPOSCHANGED:
        case WM_SIZE:
        case WM_MOVE:
            if (m_isChoiceFulfilled) {
                m_changeStyle = true;
                m_changePos = true;
                m_isChoiceFulfilled = false;
            }
        default:
            return true;
        }
    }

    void BorderlessWindow::apply() {
        log("[BorderlessWindow] Applying Borderless Window settings...");

        // Get the window we want to change the style of.
        auto wnd = g_kanan->getWindow();

        if (wnd == nullptr) {
            log("[BorderlessWindow] ERROR: Mabi's window was not available!");
            return;
        }

        // If we haven't recorded what the default style of the window is, then
        // get it.
        if (m_defaultStyle == 0) {
            m_defaultStyle = GetWindowLong(wnd, GWL_STYLE);
        }

        m_style = m_defaultStyle;

        // Make sure the monitor is valid.
        if (m_monitorChoice >= (int)m_monitors.size()) {
            m_monitorChoice = 0;
        }

        // If we were unable to enumerate any monitors then just fail I guess.
        if (m_monitors.empty()) {
            log("[BorderlessWindow] ERROR: No monitors available!");
            return;
        }

        switch (m_styleChoice) {
        case 0:
            m_changeStyle = true;
            m_changePos = false;
            break;

        case 1: {
            const auto& r = m_monitors[m_monitorChoice].workRect;

            m_x = r.left;
            m_y = r.top;
            m_w = r.right - r.left;
            m_h = r.bottom - r.top;
            m_style &= ~(WS_BORDER | WS_CAPTION | WS_THICKFRAME);

            m_changeStyle = true;
            m_changePos = true;

            break;
        }

        case 2: {
            const auto& r = m_monitors[m_monitorChoice].rect;

            m_x = r.left;
            m_y = r.top;
            m_w = r.right - r.left;
            m_h = r.bottom - r.top;
            m_style &= ~(WS_BORDER | WS_CAPTION | WS_THICKFRAME);

            m_changeStyle = true;
            m_changePos = true;

            break;
        }

        case 3: 
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
