#include "BitmapFont.hpp"

using namespace std;

namespace kanan {
    BitmapFont::BitmapFont()
        : PatchMod{ "Bitmap Font", "Uses a sharper and much faster to render font\nCredits: Blade3575" }
    {
        // Patterns originally from Abyss by Blade3575.
        addPatch("74 0D 8B 45 08 50 8B CB E8 ? ? ? ? EB 0B", 0, { 0x90, 0x90 });
        addPatch("75 3D 83 BE ? ? ? ? ?", 0, { 0xEB });
        addPatch("74 25 80 7B 73 00", 0, { 0xEB });
        addPatch("0F 84 ? ? ? ? 8B 0D ? ? ? ? E8 ? ? ? ? 57", 0, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, -1, -1, -1, -1, -1, -1, 0x90, 0x90, 0x90, 0xB0, 0x01, -1, -1, -1, -1, 0xEB });
    }
}
