#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <Windows.h>

namespace kanan {
    class RemoteModule;

    class RemoteProcess {
    public:
        RemoteProcess() = delete;
        RemoteProcess(const RemoteProcess& other) = delete;
        RemoteProcess(RemoteProcess&& other) = delete;
        RemoteProcess(DWORD processID);
        RemoteProcess(HANDLE process);
        virtual ~RemoteProcess();

        bool write(uintptr_t address, const void* data, size_t size);
        bool read(uintptr_t address, void* data, size_t size);

        bool execute(uintptr_t functionAddress, uintptr_t paramAddress);

        // 
        // Helpers that just pass a string as a parameter to a new thread.
        // 
        bool execute(uintptr_t functionAddress, const std::string& param);
        bool execute(uintptr_t functionAddress, const std::wstring& param);

        std::shared_ptr<RemoteModule> loadLibrary(const std::string& name);

        auto getHandle() const {
            return m_process;
        }

        auto isValid() const {
            return m_process != nullptr;
        }

        RemoteProcess& operator=(const RemoteProcess& other) = delete;
        RemoteProcess& operator=(RemoteProcess&& other) = delete;

    private:
        HANDLE m_process;
        std::vector<std::shared_ptr<RemoteModule>> m_modules;

        // This method can be called more than once to add any modules that were
        // newly loaded since the last time it was called.
        bool enumerateModules();

        std::shared_ptr<RemoteModule> getModule(const std::string& name);
    };
}
