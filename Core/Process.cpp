#include <thread>

#include <Windows.h>
#include <TlHelp32.h>

#include "String.hpp"
#include "Process.hpp"

using namespace std;

namespace kanan {
    optional<DWORD> getProcessID(const string& exeName) {
        auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snapshot == INVALID_HANDLE_VALUE) {
            return {};
        }

        PROCESSENTRY32 entry{};

        entry.dwSize = sizeof(entry);

        if (Process32First(snapshot, &entry) != FALSE) {
            do {
                if (lstrcmpi(entry.szExeFile, widen(exeName).c_str()) == 0) {
                    CloseHandle(snapshot);

                    return entry.th32ProcessID;
                }
            } while (Process32Next(snapshot, &entry) != FALSE);
        }

        CloseHandle(snapshot);

        return {};
    }

    DWORD waitForProcess(const string& exeName) {
        DWORD processID{ 0 };

        while (processID == 0) {
            processID = getProcessID(exeName).value_or(0);
            this_thread::yield();
        }

        return processID;
    }
}
