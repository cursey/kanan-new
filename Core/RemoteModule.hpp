#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include <Windows.h>

namespace kanan {
    class RemoteProcess;

    class RemoteModule {
    public:
        RemoteModule() = delete;
        RemoteModule(std::string name, std::string path, uintptr_t address);

        // NOTE: Only handles named exports (not exported ordinals or forwarded 
        // exports).
        std::optional<uintptr_t> getProcAddress(const std::string& procName);

        const auto& getName() const {
            return m_name;
        }

        const auto& getPath() const {
            return m_path;
        }

        auto getAddress() const {
            return m_address;
        }

        bool isValid() const {
            return m_ntHeaders != nullptr;
        }

    private:
        std::string m_name;
        std::string m_path;
        uintptr_t m_address;

        std::vector<uint8_t> m_dll;
        PIMAGE_NT_HEADERS m_ntHeaders;

        bool readInFile();
        bool verifyHeaders();
    };
}