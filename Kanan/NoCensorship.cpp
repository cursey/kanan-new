#include "NoCensorship.hpp"

namespace kanan {
    NoCensorship::NoCensorship()
        : PatchMod{ "No Censorship", "Disables the chat filter." }
    {
        addPatch("0F 84 ? ? ? ? 80 7D F3 00 0F 84 ? ? ? ? 85 D2", 0, { 0x90, 0xE9 });
    }
}
