#include <stdexcept>

#include <String.hpp>

#include "App.hpp"

using std::runtime_error;
using std::string_view;
using kanan::widen;

App* g_app{};

LRESULT CALLBACK windowProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return g_app->onMessage(wnd, msg, wParam, lParam);
}

App::App(std::string_view windowName, int w, int h)
    : m_window{},
    m_windowW{ w },
    m_windowH{ h }
{
    g_app = this;

    // Create the window class.
    WNDCLASSEX winClass{};

    winClass.cbSize = sizeof(WNDCLASSEX);
    winClass.lpfnWndProc = windowProc;
    winClass.hInstance = GetModuleHandle(nullptr);
    winClass.lpszClassName = L"KananApp";

    if (RegisterClassEx(&winClass) == FALSE) {
        throw runtime_error{ "Failed to register window class" };
    }

    // Create the window.
    m_window = CreateWindowEx(
        0,
        L"KananApp",
        widen(windowName).c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        m_windowW,
        m_windowH,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    if (m_window == nullptr) {
        throw runtime_error{ "Failed to create window" };
    }
}

App::~App() {
    if (m_window != nullptr) {
        DestroyWindow(m_window);
    }

    UnregisterClass(L"KananApp", GetModuleHandle(nullptr));
}

LRESULT App::onMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(wnd, msg, wParam, lParam);
}

void App::onFrame() {
}

void App::run() {
    ShowWindow(m_window, TRUE);
    UpdateWindow(m_window);

    // Run the message loop.
    MSG msg{};

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            onFrame();
        }
    }
}

