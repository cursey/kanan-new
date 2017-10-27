#include <Windows.h>
#include <TlHelp32.h>

#include "RemoteMemory.hpp"
#include "RemoteModule.hpp"
#include "RemoteThread.hpp"
#include "String.hpp"
#include "RemoteProcess.hpp"

using namespace std;

namespace kanan {
    RemoteProcess::RemoteProcess(DWORD processID)
        : m_process{ nullptr },
        m_modules{}
    {
        m_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    }

    RemoteProcess::RemoteProcess(HANDLE process)
        : m_process{ process },
        m_modules{}
    { }

    RemoteProcess::~RemoteProcess() {
        CloseHandle(m_process);
    }

    bool RemoteProcess::write(uintptr_t address, const void* data, size_t size) {
        SIZE_T bytesWritten{ 0 };

        return WriteProcessMemory(m_process, (LPVOID)address, data, size, &bytesWritten) != FALSE && bytesWritten == size;
    }

    bool RemoteProcess::read(uintptr_t address, void* data, size_t size) {
        SIZE_T bytesRead{ 0 };

        return ReadProcessMemory(m_process, (LPCVOID)address, data, size, &bytesRead) != FALSE && bytesRead == size;
    }

    bool RemoteProcess::execute(uintptr_t functionAddress, uintptr_t paramAddress) {
        // We start the remote thread and wait for it to finish.
        RemoteThread thread{ *this, functionAddress, paramAddress };
        DWORD exitCode{ 0 };

        return thread.isValid() &&
            WaitForSingleObject(thread.getHandle(), INFINITE) != WAIT_TIMEOUT &&
            GetExitCodeThread(thread.getHandle(), &exitCode) != FALSE;
    }

    bool RemoteProcess::execute(uintptr_t functionAddress, const string& param) {
        // Allocate memory for the param string.
        RemoteMemory paramMemory{ *this, param.length() * sizeof(char), PAGE_READWRITE };

        // Write the string there.
        if (!paramMemory.isValid() ||
            !paramMemory.write(param.c_str(), param.length() * sizeof(char))) 
        {
            return false;
        }

        // Run the thread.
        return execute(functionAddress, paramMemory.getAddress());
    }

    bool RemoteProcess::execute(uintptr_t functionAddress, const wstring& param) {
        // Allocate memory for the param string.
        RemoteMemory paramMemory{ *this, param.length() * sizeof(wchar_t), PAGE_READWRITE };

        // Write the string there.
        if (!paramMemory.isValid() || 
            !paramMemory.write(param.c_str(), param.length() * sizeof(wchar_t))) 
        {
            return false;
        }

        // Run the thread.
        return execute(functionAddress, paramMemory.getAddress());
    }

    shared_ptr<RemoteModule> RemoteProcess::loadLibrary(const string& name) {
        // If the modules list is empty (for instance, if it is the first time this
        // method is called) then enumerate the modules.
        if (m_modules.empty()) {
            enumerateModules();
        }

        // If we already know about the module then just return it.
        if (auto module = getModule(name)) {
            return module;
        }
        
        // Otherwise, try loading the module by executing a thread on LoadLibraryW.
        auto kernel32 = getModule("kernel32.dll");

        if (!kernel32) {
            return {};
        }

        auto loadLibrary = kernel32->getProcAddress("LoadLibraryW");

        if (!loadLibrary) {
            return {};
        }

        if (!execute(*loadLibrary, widen(name))) {
            return {};
        }

        // Re-enumerate the modules now that the library has been loaded.
        enumerateModules();

        return getModule(name);
    }

    bool RemoteProcess::enumerateModules() {
        auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetProcessId(m_process));

        if (snapshot == INVALID_HANDLE_VALUE) {
            return false;
        }

        MODULEENTRY32 entry{};

        entry.dwSize = sizeof(entry);

        if (Module32First(snapshot, &entry) != FALSE) {
            do {
                // Skip modules that are already known.
                if (getModule(narrow(entry.szModule))) {
                    continue;
                }

                auto module = make_shared<RemoteModule>(narrow(entry.szModule), narrow(entry.szExePath), (uintptr_t)entry.hModule);

                if (!module->isValid()) {
                    continue;
                }

                m_modules.emplace_back(module);
            } while (Module32Next(snapshot, &entry) != FALSE);
        }

        CloseHandle(snapshot);

        return true;
    }

    shared_ptr<RemoteModule> RemoteProcess::getModule(const string& name) {
        auto wideName = widen(name);

        for (auto& module : m_modules) {
            if (lstrcmpi(widen(module->getName()).c_str(), wideName.c_str()) == 0) {
                return module;
            }

            if (lstrcmpi(widen(module->getPath()).c_str(), wideName.c_str()) == 0) {
                return module;
            }
        }

        return {};
    }
}