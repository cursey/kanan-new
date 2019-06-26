#include <stdexcept>

#include <String.hpp>

#include "RegQuery.hpp"

using std::runtime_error;
using std::string;
using std::string_view;
using std::wstring;
using kanan::narrow;
using kanan::widen;

string regQueryString(HKEY key, string_view subKey, string_view valueName) {
    wstring result{};
    DWORD length{ 256 * sizeof(wchar_t) };

    result.resize(256);

    if (RegGetValue(
        key,
        widen(subKey).c_str(),
        widen(valueName).c_str(),
        RRF_RT_REG_SZ | RRF_SUBKEY_WOW6464KEY,
        nullptr,
        (LPBYTE)result.data(),
        &length
    ) != ERROR_SUCCESS) {
        throw runtime_error{ "Failed to query the registry" };
    }

    return narrow(result.substr(0, length / sizeof(wchar_t) - 1));
}

