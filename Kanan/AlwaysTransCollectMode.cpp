#include "AlwaysTransCollectMode.hpp"

using namespace std;

namespace kanan {
	AlwaysTransCollectMode::AlwaysTransCollectMode()
		: PatchMod{ "Always Trans Collect Mode", "Automatically enable Transformation Mastery's Collect Mode." }
	{
		addPatch("38 5F 78 0F 84 CF 02 00 00 8B CE", 4, { 0x85 });
	}
}
