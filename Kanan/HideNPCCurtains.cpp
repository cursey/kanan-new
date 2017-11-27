#include "HideNPCCurtains.hpp"

using namespace std;

namespace kanan {
	HideNPCCurtains::HideNPCCurtains()
		: PatchMod{ "Hide NPC Curtains", "Hide the black border curtains at the top and bottom of the screen while talking to NPCs, viewing a cutscene or using a moon gate." }
	{
		addPatch("55 8b ec 8a 45 08 56 8b f1 38 06", 0, { 0xB0, 0x00, 0xC2, 0x04, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
	}
}
