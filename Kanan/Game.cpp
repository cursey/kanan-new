#include <Scan.hpp>
#include <String.hpp>
#include <Utility.hpp>

#include "Log.hpp"
#include "Game.hpp"

using namespace std;

namespace kanan {
    Game::Game()
        : m_renderer{ nullptr },
        m_entityList{ nullptr }, 
        m_world{nullptr}, 
        m_account{nullptr}
    {
        log("Entering Game constructor.");

        // Find the games global renderer pointer.
        auto rendererAddress = scan("client.exe", "48 8B ? ? ? ? ? E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B ? ? ? ? ? 48 8B ? 48 8B ? 44 89");

        if (rendererAddress) {
            do {
                m_renderer = (CRenderer**)rel_to_abs(*rendererAddress + 3);
            } while (*m_renderer == nullptr);

            log("Got CRenderer %p", *m_renderer);
        }
        else {
            error("Failed to find address of CRenderer.");
        }

        // Find the games global entity list pointer.
        auto entityListAddress = scan("client.exe", "48 8B ? ? ? ? ? E8 ? ? ? ? 48 85 ? 0F 84 ? ? ? ? 48 8B ? ? ? ? ? 48 8B ? ? ? ? ? E8 ? ? ? ? 48 85");

        if (entityListAddress) {
            do {
                m_entityList = (CEntityList**)rel_to_abs(*entityListAddress + 3);
            } while (*m_entityList == nullptr);

            log("Got CEntityList %p", *m_entityList);
        }
        else {
            error("Failed to find CEntityList.");
        }

        // Find the games global world pointer.
        auto worldAddress = scan("client.exe", "48 8B ? ? ? ? ? E8 ? ? ? ? 84 C0 0F 85 ? ? ? ? 49 8B ? ? ? ? ? 48 85 ? 74");

        if (worldAddress) {
            do {
                m_world = (CWorld**)rel_to_abs(*worldAddress + 3);
            } while (*m_world == nullptr);

            log("Got CWorld %p", *m_world);
        }
        else {
            error("Failed to find CWorld.");
        }

         // Find the games global account pointer.
        auto accountAddress = scan("client.exe", "48 8B ? ? ? ? ? E8 ? ? ? ? 84 C0 74 ? 49 8B ? E8 ? ? ? ? 84 C0 75 ? 49 8B");

        if (accountAddress) {
            do {
                m_account = (CAccount**)rel_to_abs(*accountAddress + 3);
            } while (*m_account == nullptr);

            log("Got CAccount %p", *m_account);
        }
        else {
            error("Failed to find CAccount.");
        }

        log("Leaving Game constructor.");
    }

    KCharacter* Game::getCharacterByID(uint64_t id) {
        auto entityList = getEntityList();

        if (entityList == nullptr) {
            return nullptr;
        }

        // Go through the entity list looking for a character with the matching
        // ID.
        auto& characters = entityList->characters;
        auto highestIndex = characters.count;
        auto node = *characters.root;

        for (uint32_t i = 0; i < highestIndex && node != nullptr; ++i, node = node->next) {
            auto character = (KCharacter*)node->character;

            if (character == nullptr) {
                continue;
            }

            auto characterID = character->getID();

            if (!characterID || *characterID != id) {
                continue;
            }

            return character;
        }

        // If we get here then no character with the specified ID is in the list.
        return nullptr;
    }

    KCharacter* Game::getLocalCharacter() {
        auto world = getWorld();

        if (world == nullptr) {
            return nullptr;
        }

        return getCharacterByID(world->localPlayerID);
    }

    void Game::changeChannel(int channel) {
        static auto cc = (char(__thiscall*)(void*, CString**, char, char))scan("client.exe", "55 8B EC 6A ? 68 ? ? ? ? 64 A1 ? ? ? ? 50 83 EC ? 53 56 57 A1 ? ? ? ? 33 C5 50 8D 45 F4 64 A3 ? ? ? ? 8B D9 A1 ? ? ? ? 83 78 20 ?").value_or(0);
        static auto logged = false;

        if (!logged) {
            log("Change channel function = %p", cc);
            logged = true;
        }
        //patching to allow to work on nao
        if (channel < 1 || channel > 10) {
            return;
        }

        auto account = getAccount();

        if (!account) {
            return;
        }

        log("Account = %p", account);

        string str{ "Ch" };

        str += to_string(channel);

        log("Channel = %s", str.c_str());

        CString val{};

        // CMessage::WriteString only needs these members.
        val.capacity = sizeof(val.buffer) / 2;
        val.length = str.length();
        val.referenceCount = 1;

        wcscpy_s((wchar_t*)val.buffer, val.capacity, widen(str).c_str());

        auto val2 = &val;

        cc(account, &val2, 0, 0);
    }
}
