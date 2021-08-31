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

        CRenderer* getRenderer() const;
        CEntityList* getEntityList() const;
        CWorld* getWorld() const;
        CAccount* getAccount() const;

    private:
        CRendererPtr* m_rendererPtr;
        CEntityListPtr* m_entityListPtr;
        CWorldPtr* m_worldPtr;
        CAccountPtr* m_accountPtr;
    };
}
