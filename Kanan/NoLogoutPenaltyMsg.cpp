#include "NoLogoutPenaltyMsg.hpp"

namespace kanan {
    NoLogoutPenaltyMsg::NoLogoutPenaltyMsg()
        : PatchMod{ "No Logout Penalty Msg", "Automatically skips the \"If you exit now, you will lose 12911 EXP and 3131 Gold.\" warning message." }
    {
        addPatch("0F 84 ? ? ? ? 88 5D F2 88 5D F3", 0, { 0x90, 0xE9 });
    }
}
