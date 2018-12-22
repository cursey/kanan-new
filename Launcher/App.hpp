#pragma once

#include <string_view>

#include <Windows.h>

class App {
public:
    App(std::string_view windowName, int w, int h);
    virtual ~App();

    virtual LRESULT onMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
    virtual void onFrame();

    void run();

    auto getWindow() {
        return m_window;
    }

    auto getWindowW() const {
        return m_windowW;
    }

    auto getWindowH() const {
        return m_windowH;
    }

protected:
    HWND m_window;
    int m_windowW;
    int m_windowH;
};

