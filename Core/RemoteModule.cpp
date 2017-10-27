#include <fstream>

#include "Module.hpp"
#include "RemoteModule.hpp"

using namespace std;

namespace kanan {
    RemoteModule::RemoteModule(string name, string path, uintptr_t address)
        : m_name{ move(name) },
        m_path{ move(path) },
        m_address{ address }
    {
        // The onlything the constructor needs to do is read in the file data for
        // this dll, and verify the image headers.
        readInFile();
        verifyHeaders();
    }

    optional<uintptr_t> RemoteModule::getProcAddress(const string& procName) {
        if (m_ntHeaders == nullptr) {
            return {};
        }

        // Get the export data directory.
        auto exportData = &m_ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

        if (exportData == nullptr || exportData->Size == 0) {
            return {};
        }

        // Get the export directory.
        auto dll = m_dll.data();
        auto exportDirectory = (PIMAGE_EXPORT_DIRECTORY)ptrFromRVA(dll, exportData->VirtualAddress).value_or(0);

        if (exportDirectory == nullptr) {
            return {};
        }

        auto functions = (uint32_t*)ptrFromRVA(dll, exportDirectory->AddressOfFunctions).value_or(0);
        auto names = (uint32_t*)ptrFromRVA(dll, exportDirectory->AddressOfNames).value_or(0);
        auto ordinals = (uint16_t*)ptrFromRVA(dll, exportDirectory->AddressOfNameOrdinals).value_or(0);

        if (functions == nullptr || names == nullptr || ordinals == nullptr) {
            return {};
        }

        uintptr_t address{ 0 };

        // Find the named export. 
        for (uint32_t i = 0; i < exportDirectory->NumberOfNames; ++i) {
            auto name = (char*)ptrFromRVA(dll, names[i]).value_or(0);

            if (name == nullptr) {
                continue;
            }

            if (strcmp(name, procName.c_str()) == 0) {
                address = functions[ordinals[i]];
            }
        }

        if (address == 0) {
            return {};
        }

        return m_address + address;
    }

    bool RemoteModule::readInFile() {
        // Open the file at the end so we can quickly get the size of it (ios::ate).
        ifstream file(m_path, ios::in | ios::binary | ios::ate);

        if (!file.is_open()) {
            return false;
        }

        m_dll.resize((size_t)file.tellg());

        // Reposition the indicator to the beggining of the file and read the contents.
        file.seekg(0, ios::beg);
        file.read((char*)m_dll.data(), m_dll.size());

        return true;
    }

    bool RemoteModule::verifyHeaders() {
        if (m_dll.empty()) {
            return false;
        }

        // Verify the dos header signature.
        auto dosHeader = (PIMAGE_DOS_HEADER)m_dll.data();

        if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
            return false;
        }

        // Verify the nt headers signature.
        auto ntHeaders = (PIMAGE_NT_HEADERS)(m_dll.data() + dosHeader->e_lfanew);

        if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
            return false;
        }

        // Everything checks out, keep the pointer to the nt headers around since
        // its used in other methods.
        m_ntHeaders = ntHeaders;

        return true;
    }
}