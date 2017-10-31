#include <cstdarg>

#include <Windows.h>

#include <String.hpp>

#include "Kanan.hpp"
#include "Log.hpp"

using namespace std;

namespace kanan {
    static ofstream g_log{};

    void startLog(const string& filepath) {
        g_log.open(filepath);

        if (!g_log.is_open()) {
            error("Failed to open log file: {}!", filepath);
        }
    }

    void log(const string& msg) {
        if (g_log.is_open()) {
            g_log << msg << endl;
        }
    }

    void msg(const string& msg) {
        log(msg);

        // Use the real window if we have it.
        HWND wnd{ nullptr };

        if (g_kanan) {
            wnd = g_kanan->getWindow();
        }
        else {
            wnd = GetDesktopWindow();
        }

        MessageBox(wnd, widen(msg).c_str(), L"Kanan", MB_ICONINFORMATION | MB_OK);
    }

    void error(const string& msg) {
        log(msg);

        // Use the real window if we have it.
        HWND wnd{ nullptr };

        if (g_kanan) {
            wnd = g_kanan->getWindow();
        }
        else {
            wnd = GetDesktopWindow();
        }

        MessageBox(wnd, widen(msg).c_str(), L"Kanan Error!", MB_ICONERROR | MB_OK);
    }

    void log(const char* format, ...) {
        va_list args{};

        va_start(args, format);
        log(formatString(format, args));
        va_end(args);
    }

    void msg(const char* format, ...) {
        va_list args{};

        va_start(args, format);
        msg(formatString(format, args));
        va_end(args);
    }

    void error(const char* format, ...) {
        va_list args{};

        va_start(args, format);
        error(formatString(format, args));
        va_end(args);
    }
}
