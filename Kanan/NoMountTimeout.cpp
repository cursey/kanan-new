#include "NoMountTimeout.hpp"

using namespace std;

namespace kanan {
    NoMountTimeout::NoMountTimeout()
        : PatchMod{ "No Mount Timeout", "Lets you mount/demount your pet as fast as you want\nCredits: Licat" }
    {
        addPatch("89 90 58 02 00 00", 0, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
    }
}
