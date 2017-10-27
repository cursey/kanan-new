#include <Windows.h>

#include "String.hpp"

using namespace std;

namespace kanan {
    string narrow(const wstring& str) {
        auto length = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)str.length(), nullptr, 0, nullptr, nullptr);
        string narrowStr{};

        narrowStr.resize(length);
        WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)str.length(), (LPSTR)narrowStr.c_str(), length, nullptr, nullptr);

        return narrowStr;
    }

    wstring widen(const string& str) {
        auto length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), nullptr, 0);
        wstring wideStr{};

        wideStr.resize(length);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), (LPWSTR)wideStr.c_str(), length);

        return wideStr;
    }
}