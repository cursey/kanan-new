#include "HideObjects.hpp"

using namespace std;

namespace kanan {
	HideObjects::HideObjects()
		: PatchMod{ "Hide Objects", "Stops all objects from loading." }
	{
		addPatch("39 78 0C 0F 94 C0 84 C0 0F 84 92 00 00 00 8B 45", 8, { 0x90,0xE9 });
	}
}
