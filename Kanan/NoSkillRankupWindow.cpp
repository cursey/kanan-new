#include "NoSkillRankupWindow.hpp"

namespace kanan {
    NoSkillRankupWindow::NoSkillRankupWindow()
        : PatchMod{ "No Skill Rankup Window", "Skips the popup after you advance a skill." }
    {
        addPatch("8b f0 33 db 3b f3 75 16 32 c0", 6, { 0x90, 0x90 });
        addPatch("e8 ? ? ? ? 85 c0 0f 84 ? ? ? ? 8b 10 8b 35", 7, { 0x90, 0xE9 });
    }
}
