#include "SelfRightClickable.hpp"

using namespace std;

namespace kanan {
	SelfRightClickable::SelfRightClickable()
		: PatchMod{ "Self Right Clickable", "This lets you right click yourself." }
	{
		addPatch("0F 85 ? ? ? ? 3B D3 0F 85 ? ? ? ? 8B 8E ? ? ? ? E8 ? ? ? ? 84 C0 74 4A", 0, { 0x90, 0xE9 });
	}
}
