#include "RemoteProcess.hpp"
#include "RemoteMemory.hpp"

using namespace std;

namespace kanan {
    RemoteMemory::RemoteMemory(RemoteProcess& process, size_t size, DWORD protection)
        : m_process{ process },
        m_address{ 0 },
        m_size{ size },
        m_doCleanup{ true }
    {
        m_address = (uintptr_t)VirtualAllocEx(m_process.getHandle(), nullptr, m_size, MEM_COMMIT | MEM_RESERVE, protection);
    }

    RemoteMemory::~RemoteMemory() {
        if (m_doCleanup) {
            free();
        }
    }

    bool RemoteMemory::free() {
        // Don't attempt to free invalid memory.
        if (m_address == 0) {
            return true;
        }

        if (VirtualFreeEx(m_process.getHandle(), (LPVOID)m_address, 0, MEM_RELEASE) == FALSE) {
            return false;
        }

        // Invalidate this memory.
        m_address = 0;
        m_size = 0;

        return true;
    }

    void RemoteMemory::dontFree() {
        m_doCleanup = false;
    }

    bool RemoteMemory::write(const void * data, size_t size) {
        return write(0, data, size);
    }

    bool RemoteMemory::read(void * data, size_t size) {
        return read(0, data, size);
    }

    bool RemoteMemory::write(unsigned int offset, const void* data, size_t size) {
        // Make sure we are within the bounds of our memory.
        if (offset + size > m_size) {
            return false;
        }

        SIZE_T bytesWritten{ 0 };

        return WriteProcessMemory(m_process.getHandle(), (LPVOID)(m_address + offset), data, size, &bytesWritten) != FALSE && bytesWritten == size;
    }

    bool RemoteMemory::read(unsigned int offset, void* data, size_t size) {
        // Make sure we are within the bounds of our memory.
        if (offset + size > m_size) {
            return false;
        }

        SIZE_T bytesRead{ 0 };

        return ReadProcessMemory(m_process.getHandle(), (LPCVOID)(m_address + offset), data, size, &bytesRead) != FALSE && bytesRead == size;
    }
}