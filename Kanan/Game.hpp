#pragma once

#include <optional>

#include "Mabinogi.hpp"
#include "KCharacter.hpp"

namespace kanan {
    // This object represents the interface between kanan and the game's systems and
    // structures.
    class Game {
    public:
        Game();

        KCharacter* getCharacterByID(uint64_t id);
        KCharacter* getLocalCharacter();

        void changeChannel(int channel);

        CRenderer* getRenderer() const { return m_renderer; }
        CEntityList* getEntityList() const { return m_entityList; }
        CWorld* getWorld() const { return m_world; }
        CAccount* getAccount() const { return m_account; }

    private:
        CRenderer* m_renderer;
        CEntityList* m_entityList;
        CWorld* m_world;
        CAccount* m_account;
    };
}
