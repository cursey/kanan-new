#include "MiniTitleMenu.hpp"

using namespace std;

namespace kanan {
	MiniTitleMenu::MiniTitleMenu()
		: PatchMod{ "Mini Title Menu", "Removes all unknown titles (???) in the title selection menu." }
	{
		addPatch("85 C0 0F 88 ? ? ? ? 83 F8 01 7E ? 83 F8 02 0F 85 ? ? ? ? 8B 0D", 3, { 0x8A });
		addPatch("85 C0 0F 88 ? ? ? ? 83 F8 01 7E ? 83 F8 02 0F 85 ? ? ? ? 8B 0D", 11, { 0x7A });
		addPatch("85 C0 0F 88 ? ? ? ? 83 F8 01 7E ? 83 F8 02 0F 85 ? ? ? ? 8B 0D", 17, { 0x84 });
	}
}
