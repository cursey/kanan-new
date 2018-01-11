#pragma once

#include <forward_list>

#include "Mod.hpp"

namespace kanan {
    class KCharacter;
    class KItem;

    class EntityViewer : public Mod {
    public:
        void onUI() override;

    private:
        std::forward_list<KCharacter*> m_characters;
        std::forward_list<KItem*> m_items;

        void buildCharacterList();
        void buildItemList();

        void createCharacterTree();
        void createItemTree();

        void displayCharacter(KCharacter* character);
        void displayItem(KItem* item);

		double durabilityToDouble(uint32_t dura, uint32_t maxDura);
		double durabilityToDouble(uint32_t maxDura);
		char* raceToString(uint32_t raceType);
    };
}
