#include "RemoteProcess.hpp"
#include "RemoteThread.hpp"

using namespace std;

namespace kanan {
    RemoteThread::RemoteThread(RemoteProcess& process, uintptr_t functionAddress, uintptr_t paramAddress)
        : m_thread{ nullptr }
    {
        if (functionAddress == 0) {
            return;
        }

        DWORD threadID{ 0 };

        m_thread = CreateRemoteThread(process.getHandle(), nullptr, 0, (LPTHREAD_START_ROUTINE)functionAddress, (LPVOID)paramAddress, 0, &threadID);
    }

    RemoteThread::~RemoteThread() {
        CloseHandle(m_thread);
    }
}
