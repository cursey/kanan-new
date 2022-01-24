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
    HKEY key_handle{};

    auto status = RegOpenKeyEx(key, widen(subKey).c_str(), 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &key_handle);

    if (status != ERROR_SUCCESS) {
        throw runtime_error{ "Failed to open the registry" };
    }

    wstring result{};
    DWORD length{ 256 * sizeof(wchar_t) };
    DWORD type = REG_SZ;

    result.resize(256);

    status = RegQueryValueEx(key_handle, widen(valueName).c_str(), 0, &type, (LPBYTE)result.data(), &length);

    RegCloseKey(key_handle);

    if (status != ERROR_SUCCESS) {
        throw runtime_error{ "Failed to query the registry" };
    }

    return narrow(result.substr(0, length / sizeof(wchar_t) - 1));
}

