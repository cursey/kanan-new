#include "ShowItemClass.hpp"

using namespace std;

namespace kanan {
	ShowItemClass::ShowItemClass()
		: PatchMod{ "Show Item Class", "Toggles on the dev item class ID and attribute flag view." }
	{
		addPatch("66 3B C1 0F 85 ? 02 00 00 68 ? ? ? ? B9", 3, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
	}
}
