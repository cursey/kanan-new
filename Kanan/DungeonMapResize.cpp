#include "DungeonMapResize.hpp"

using namespace std;

namespace kanan {
	DungeonMapResize::DungeonMapResize()
		: PatchMod{ "Dungeon Map Resize", "Disables the restriction of resizing the map while inside dungeons." }
	{
		addPatch("C2 8D ? ? ? ? ? ? B8 B4 00 00 00", 9, { 0xFF });
		addPatch("BF B4 00 00 00 6A", 1, { 0x19 });
	}
}
