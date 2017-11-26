#include "PartyNoClose.hpp"

namespace kanan {
	PartyNoClose::PartyNoClose() : SimpleToggleMod("PartyNoClose", "Keep Party Ads Open", "Keeps your party always open until you close it.")
	{
		addPatch("55 8B EC 56 8B F1 8B 46 64 83 78 08 00 74 32", 13, { 0xEB });
	}
}