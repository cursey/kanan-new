#include "DisableScreenFlash.hpp"

using namespace std;

namespace kanan {
	DisableScreenFlash::DisableScreenFlash()
		: PatchMod{ "Disable Screen Flash", "Disables white screen flashes from occuring while loading or executing certain skills." }
	{
		addPatch("55 1C 53 ? ? ? ? ? ? ? ? 56", 2, { 0x50 });
	}
}
