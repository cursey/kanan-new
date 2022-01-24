#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "DontMoveToSquadChat.hpp"

using namespace std;

namespace kanan {
    DontMoveToSquadChat::DontMoveToSquadChat()
        : PatchMod{ "Don't Move To Squad Chat", "Goes to All chat instead of Squad chat." },
        m_isEnabled{ false },
        m_patch{},
        m_originalByte{}
    {
        auto address = scan("client.exe", "89 BB 10 01 00 00 FF 90");

        if (address) {
            log("Got DontMoveToSquadChat %p", *address);

            code = vector<uint8_t>{
                0x83, 0xFF, 0x08,						//cmp edi,08
                0x0F, 0x85, 0x02, 0x00, 0x00, 0x00,		//jne nochange
                0x31, 0xFF,								//xor edi,edi
                                                        //nochange:
                0x89, 0xBB, 0x10, 0x01, 0x00, 0x00,		//mov [ebx+00000110],edi
                0xE9, 0x00, 0x00, 0x00, 0x00            //jmp back to code
            };

            m_patch.address = *address;
        }
        else {
            log("Failed to find DontMoveToSquadChat address.");
        }
    }

    void DontMoveToSquadChat::onPatchUI() {
        if (m_patch.address == 0) {
            return;
        }

        if (ImGui::Checkbox("Don't Move To Squad Chat", &m_isEnabled)) {
            apply();
        }
    }

    void DontMoveToSquadChat::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("DontMoveToSquadChat.Enabled").value_or(false);
    }

    void DontMoveToSquadChat::onConfigSave(Config& cfg) {
        cfg.set<bool>("DontMoveToSquadChat.Enabled", m_isEnabled);
    }

    void DontMoveToSquadChat::apply() {
        if (m_patch.address == 0) {
            return;
        }

        if (m_isEnabled) {
            log("Applying DontMoveToSquadChat...");

            code_address = VirtualAlloc(nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

            if (code_address == NULL) {
                log("Failed to allocate memory for DontMoveToSquadChat.");
                return;
            }
            else {
                log("Successfully allocated memory for DontMoveToSquadChat %p", (uintptr_t)code_address);
            }

            memcpy(code_address, code.data(), code.size());

            //because we are using a relative jmp we can calculate the offset to get the bytes
            uintptr_t jmpToBytes = (uintptr_t)code_address - (m_patch.address + 5);

            //convert the address holding the bytes into a byte array
            unsigned char jmpToArray[sizeof(void*)];
            memcpy(jmpToArray, &jmpToBytes, sizeof(void*));

            m_patch.bytes = { 0xE9, jmpToArray[0], jmpToArray[1], jmpToArray[2], jmpToArray[3], 0x90 };

            //now we calculate the jmp back
            uintptr_t jmpFromBytes = m_patch.address - ((uintptr_t)code_address + (code.size() - m_patch.bytes.size()));

            unsigned char jmpFromArray[sizeof(void*)];
            memcpy(jmpFromArray, &jmpFromBytes, sizeof(void*));

            auto jmpBack = vector<uint8_t>{ 0xE9, jmpFromArray[0], jmpFromArray[1], jmpFromArray[2], jmpFromArray[3] };
            uintptr_t jmpBackAddress = (uintptr_t)code_address + (code.size() - jmpBack.size());
            memcpy((void *)jmpBackAddress, jmpBack.data(), jmpBack.size());

            patch(m_patch);
        }
        else {
            log("Disabling DontMoveToSquadChat...");

            undoPatch(m_patch);
            VirtualFree(code_address, 0, MEM_RELEASE);
        }
    }
}
