#pragma once

#include <fstream>

namespace kanan {
    extern std::ofstream g_log;

    void msg(const std::string& msg);
    void errorMsg(const std::string& msg);
}