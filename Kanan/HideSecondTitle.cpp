#include "HideSecondTitle.hpp"

using namespace std;

namespace kanan {
	HideSecondTitle::HideSecondTitle()
		: PatchMod{ "Hide Second Title", "Hide away the graphical icon display of secondary titles worn by players." }
	{
		addPatch("C1 E9 14 F6 C1 01 75 ? 8B", 6, { 0xEB });
	}
}
