#include "ShowCombatPower.hpp"

using namespace std;

namespace kanan {
    ShowCombatPower::ShowCombatPower()
        : PatchMod{ "Show Combat Power", "Shows the combat power of players and enemies\nCredits: Rydian" }
    {
        addPatch("0F 85 ? ? ? ? 8B 97 ? ? ? ? 8B 87", 0, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
    }
}
