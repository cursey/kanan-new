#include "ElfLagFix.hpp"

using namespace std;

namespace kanan {
    ElfLagFix::ElfLagFix()
        : PatchMod{ "Elf Lag Fix", "Removes the lag elves have.\nCredits: Blade3575" }
    {
        addPatch("55 8B EC 56 57 8B F9 8B 07 8B 50 0C FF D2 8B C8", 0, { 0x32, 0xC0, 0xC2, 0x04, 0x00 });
    }
}

