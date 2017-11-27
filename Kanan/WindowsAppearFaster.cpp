#include "WindowsAppearFaster.hpp"

using namespace std;

namespace kanan {
    WindowsAppearFaster::WindowsAppearFaster()
        : PatchMod{ "Windows Appear Faster", "UI windows will appear faster\nCredits: Step29" }
    {
        addPatch("74 6F E8 ? ? ? ? 85 C0 74 10", 0, { 0xEB });
        addPatch("8B 4D 18 6A 00 51 8B 8E ? ? ? ?", 0, { 0x31, 0xC9, 0x90 });
    }
}
