#include "NPCFastText.hpp"

using namespace std;

namespace kanan {
	NPCFastText::NPCFastText()
		: PatchMod{ "NPC Fast Text", "Speed up dialogues while chatting to NPCs to instant." }
	{
		addPatch("8B 08 33 FF 3B CF 76 2A", 6, { 0xEB });
	}
}
