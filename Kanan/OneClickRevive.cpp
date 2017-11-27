#include "OneClickRevive.hpp"

using namespace std;

namespace kanan {
    OneClickRevive::OneClickRevive()
        : PatchMod{ "One Click Revive", "Let's you revive by just clicking 'Revive Here' without the confirmation dialog\nCredits: Step29" }
    {
        addPatch("0F 86 ? ? ? ? 8B 4D E8 8B 11", 0, { 0x90, 0xE9 });
    }
}
