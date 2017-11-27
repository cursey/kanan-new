#include "ShowItemTradePrice.hpp"

using namespace std;

namespace kanan {
	ShowItemTradePrice::ShowItemTradePrice()
		: PatchMod{ "Show Item Trade Price", "Toggles on the dev item class ID and attribute flag view." }
	{
		addPatch("0F 85 ? ? ? ? 68 ? ? ? ? 8D 4D F0 E8 ? ? ? ? 68 ? ? ? ? 8D 4D D0", 0, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
	}
}
