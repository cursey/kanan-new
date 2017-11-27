#include "ViewNPCEquipment.hpp"

using namespace std;

namespace kanan {
	ViewNPCEquipment::ViewNPCEquipment()
		: PatchMod{ "View NPC Equipment", "Adds the option \"View Equipment\" while mouse right-clicking NPCs." }
	{
		addPatch("84 C0 0F 85 A7 ? ? ? 8B CF", 0, { 0x38 });
		addPatch("84 C0 0F 85 A7 ? ? ? 8B CF", 3, { 0x8B });
	}
}
