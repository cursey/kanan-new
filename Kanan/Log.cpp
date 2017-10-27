#include <Windows.h>

#include <String.hpp>

#include "Kanan.hpp"
#include "Log.hpp"

using namespace std;

namespace kanan {
    ofstream g_log{}; 
    
    void msg(const string& msg) {
        g_log << msg << endl;

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

    void errorMsg(const string& msg) {
        g_log << msg << endl;

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


}