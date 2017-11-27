#include "ShowDeadlyHP.hpp"

using namespace std;

namespace kanan {
	ShowDeadlyHP::ShowDeadlyHP()
		: PatchMod{ "Show Deadly HP", "Unveil the player character's negative HP instead of just being displayed as DEADLY." }
	{
		addPatch("75 18 68 ? ? ? ? 8D 8D", 0, { 0xEB });
		addPatch("75 07 68 ? ? ? ? EB 1F 6A 0A 68 ? ? ? ? 8D 8D ? ? ? ? 51 53", 0, { 0xEB });
		addPatch("75 07 68 ? ? ? ? EB 1F 6A 0A 68 ? ? ? ? 8D 8D ? ? ? ? 51 50", 0, { 0xEB });
		addPatch("33 DB 85 F6 75 1C", 0, { 0x90, 0x90 });
		addPatch("33 DB 85 FF 75 0B", 0, { 0x90, 0x90 });
		addPatch("C7 85 ? ? ? ? ? ? ? ? 8B 85 ? ? ? ? 85 F6", 0, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
	}
}
