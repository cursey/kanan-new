#include <array>
#include <cstdint>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>
#include <unordered_set>

#include <Windows.h>

#include "String.hpp"
#include "Process.hpp"
#include "RemoteModule.hpp"
#include "RemoteProcess.hpp"

using namespace std;
using namespace std::experimental::filesystem;
using namespace kanan;

// We keep a set of which process ID's we've injected into.
unordered_set<DWORD> g_injectedIDs{};

bool inject(DWORD clientID) {
    cout << "Opening process..." << endl;

    RemoteProcess client{ clientID };

    if (!client.isValid()) {
        cerr << "Failed to open process." << endl;
        return false;
    }

    // Enable loading of other DLLs into Mabinogi alongside Kanan.
    ifstream dllConfig{ "Loader.txt" }; // Chose this name to make it obvious.
    vector<string> dllNames{};
    string dllConfigLine{};

    while (getline(dllConfig, dllConfigLine)) {
        dllNames.push_back(move(dllConfigLine));
    }

    for (auto& dllName : dllNames) {
        auto dllPath = current_path() / dllName;
        
        if (!exists(dllPath)) {
            cerr << "Couldn't find " << dllPath << " to inject. Skipping..." << endl;
            continue;
        }

        cout << "Injecting " << dllPath << "..." << endl;

        auto dll = client.loadLibrary(dllPath.string());

        if (!dll) {
            cerr << "Failed to inject." << endl;
            return false;
        }

        cout << "Success!" << endl;
    }

    return true;
}

BOOL CALLBACK enumWindow(HWND wnd, LPARAM param) {
    UNREFERENCED_PARAMETER(param);

    // This is really not a time sensitive matter so we yeild to let the processor
    // do more important things.
    this_thread::yield();

    array<wchar_t, 256> className{ { 0 } };

    if (GetClassName(wnd, className.data(), (int)className.size()) == 0) {
        return TRUE;
    }

    // Skip non-mabinogi windows.
    if (narrow(className.data()) != "Mabinogi") {
        return TRUE;
    }

    DWORD clientID{ 0 };

    GetWindowThreadProcessId(wnd, &clientID);

    // Skip mabinogi's we've already injected into.
    if (g_injectedIDs.count(clientID) != 0) {
        return TRUE;
    }

    g_injectedIDs.emplace(clientID);

    inject(clientID);

    cout << "Waiting for new Mabinogi window..." << endl;

    return TRUE;
}

int main(int argc, char* argv[]) {
    cout << "Kanan for Mabinogi" << endl;
    cout << endl;
    cout << "Waiting for Mabinogi window(s)..." << endl;

    // All we do is enumerate the top-level windows using EnumWindows. Then if we
    // find a window belonging to Mabinogi, we get its process ID. If we haven't
    // injected into that process, we inject then repeat the process constantly
    // looking for new instances of Mabinogi to inject into.
    do {
        EnumWindows(&enumWindow, 0);
        this_thread::sleep_for(1s);
    } while (true);

    return 0;
}
