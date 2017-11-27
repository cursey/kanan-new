#include "HotkeyAnyItem.hpp"

using namespace std;

namespace kanan {
	HotkeyAnyItem::HotkeyAnyItem()
		: PatchMod{ "Hotkey Any Item", "Allows not just potions to be placed on the hotkey bar, but ANY sort of usable item." }
	{
		addPatch("84 C0 74 06 B0 01 5D C2 04 00 32 C0 5D C2 04 00 CC", 10, { 0xB0, 0x01 });
	}
}
