#pragma once

#include <fstream>

namespace kanan {
    void startLog(const std::string& filepath);

    void log(const char* format, ...);
    void msg(const char* format, ...);
    void error(const char* format, ...);
}