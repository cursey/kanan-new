#pragma once

#include <string>

namespace kanan {
    //
    // String utilities.
    //

    // Conversion functions for UTF8<->UTF16.
    std::string narrow(const std::wstring& str);
    std::wstring widen(const std::string& str);

    std::string formatString(const char* format, va_list args);
}
