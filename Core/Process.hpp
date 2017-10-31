#pragma once

#include <optional>
#include <string>

#include <Windows.h>

namespace kanan {
    //
    // Process utilities.
    //

    // Returns the process ID of a process with a matching exe name.
    std::optional<DWORD> getProcessID(const std::string& exeName);
    DWORD waitForProcess(const std::string& exeName);
}
