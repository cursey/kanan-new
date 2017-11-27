#include <Scan.hpp>

#include "Log.hpp"
#include "Game.hpp"

using namespace std;

namespace kanan {
    Game::Game()
        : m_rendererPtr{ nullptr },
        m_entityListPtr{ nullptr },
        m_worldPtr{ nullptr },
        m_accountPtr{ nullptr }
    {
        log("Entering Game constructor.");

        // Find the games global renderer pointer.
        auto rendererAddress = scan("client.exe", "8B 0D ? ? ? ? 8D 55 C8 52 8D 45 DC");

        if (rendererAddress) {
            m_rendererPtr = *(CRendererPtr**)(*rendererAddress + 2);

            log("Got CRendererPtr %p", m_rendererPtr);
        }
        else {
            error("Failed to find address of CRendererPtr.");
        }

        // Find the games global entity list pointer.
        auto entityListAddress = scan("client.exe", "8B 0D ? ? ? ? 57 50 E8 ? ? ? ? 3B C3");

        if (entityListAddress) {
            m_entityListPtr = *(CEntityListPtr**)(*entityListAddress + 2);

            log("Got CEntityListPtr %p", m_entityListPtr);
        }
        else {
            error("Failed to find CEntityListPtr.");
        }

        // Find the games global world pointer.
        auto worldAddress = scan("client.exe", "8B 15 ? ? ? ? 8B 4A 1C E8 ? ? ? ? 0F B6 C0");

        if (worldAddress) {
            m_worldPtr = *(CWorldPtr**)(*worldAddress + 2);

            log("Got CWorldPtr %p", m_worldPtr);
        }
        else {
            error("Failed to find CWorldPtr.");
        }

        // find the games global account pointer.
        auto accountAddress = scan("client.exe", "8B 0D ? ? ? ? 6A 00 53 E8 ? ? ? ? 8B 06");

        if (accountAddress) {
            m_accountPtr = *(CAccountPtr**)(*accountAddress + 2);

            log("Got CAccountPtr %p", m_accountPtr);
        }
        else {
            error("Failed to find CAccountPtr.");
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
        auto node = characters.root;

        for (uint32_t i = 0; i < highestIndex && node != nullptr; ++i, node = node->next) {
            auto character = (KCharacter*)node->entry->character;

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

    CRenderer* Game::getRenderer() const {
        if (m_rendererPtr == nullptr || m_rendererPtr->renderer == nullptr) {
            return nullptr;
        }

        return m_rendererPtr->renderer;
    }

    CEntityList* Game::getEntityList() const {
        if (m_entityListPtr == nullptr || m_entityListPtr->entityList == nullptr) {
            return nullptr;
        }

        return m_entityListPtr->entityList;
    }

    CWorld* Game::getWorld() const {
        if (m_worldPtr == nullptr || m_worldPtr->world == nullptr) {
            return nullptr;
        }

        return m_worldPtr->world;
    }

    CAccount* Game::getAccount() const {
        if (m_accountPtr == nullptr || m_accountPtr->account == nullptr) {
            return nullptr;
        }

        return m_accountPtr->account;
    }
}
