#include "NoPersonalShopCreationMsg.hpp"

using namespace std;

namespace kanan {
	NoPersonalShopCreationMsg::NoPersonalShopCreationMsg()
		: PatchMod{ "No Personal Shop Creation Message", "Automatically confirms the \"A shop has been created.\"" }
	{
		addPatch("6A 01 53 53 6A 01 53 6A 01 53 53 8D 55 E8 52 E8 ? ? ? ? C6 45 FC 04", 0, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }); //20 nops
	}
}
