#pragma once

#include <Windows.h>

namespace kanan {
    class RemoteProcess;

    class RemoteThread {
    public:
        RemoteThread() = delete;
        RemoteThread(const RemoteThread& other) = delete;
        RemoteThread(RemoteThread&& other) = delete;
        RemoteThread(RemoteProcess& process, uintptr_t functionAddress, uintptr_t paramAddress);
        virtual ~RemoteThread();

        auto getHandle() const {
            return m_thread;
        }

        auto isValid() const {
            return m_thread != nullptr;
        }

        RemoteThread& operator=(const RemoteThread& other) = delete;
        RemoteThread& operator=(RemoteThread&& other) = delete;

    private:
        HANDLE m_thread;
    };
}
