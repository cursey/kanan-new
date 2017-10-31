#pragma once

#include <cstdint>

#include <Windows.h>

namespace kanan {
    class RemoteProcess;

    class RemoteMemory {
    public:
        RemoteMemory() = delete;
        RemoteMemory(const RemoteMemory& other) = delete;
        RemoteMemory(RemoteMemory&& other) = delete;
        RemoteMemory(RemoteProcess& process, size_t size, DWORD protection = PAGE_EXECUTE_READWRITE);
        virtual ~RemoteMemory();

        // You can explicitly free the memory if you want, otherwise it will happen
        // automatically when the object is destroyed.
        bool free();

        // Call this if you don't want the memory to be freed after the object is
        // destroyed.
        void dontFree();

        bool write(const void* data, size_t size);
        bool read(void* data, size_t size);

        bool write(unsigned int offset, const void* data, size_t size);
        bool read(unsigned int offset, void* data, size_t size);

        auto getAddress() const {
            return m_address;
        }

        auto getSize() const {
            return m_size;
        }

        auto isValid() const {
            return m_address != 0;
        }

        RemoteMemory& operator=(const RemoteMemory& other) = delete;
        RemoteMemory& operator=(RemoteMemory&& other) = delete;

    private:
        RemoteProcess& m_process;
        uintptr_t m_address;
        size_t m_size;
        bool m_doCleanup;
    };
}
