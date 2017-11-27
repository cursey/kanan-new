#include "TalkToEgo.hpp"

using namespace std;

namespace kanan {
	TalkToEgo::TalkToEgo()
		: PatchMod{ "Talk to Unequipped Ego", "Talk to unequipped ego." }
	{
		addPatch("0F 85 90 00 00 00 68 ? ? ? ? 8D 4D DC E8", 0, { 0x90, 0xE9 });
	}
}
