#include "RemoveDungeonFog.hpp"

namespace kanan {
    RemoveDungeonFog::RemoveDungeonFog()
        : PatchMod{ "Remove Dungeon Fog", "Remove the fog on the map while in dungeons, automatically unveiling them." }
    {
        addPatch("0F B6 41 05 C1 E8 02 83 E0 01 C3", 0, { 0x33, 0xC0, 0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
    }
}
