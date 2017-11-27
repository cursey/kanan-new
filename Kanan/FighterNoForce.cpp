#include "FighterNoForce.hpp"

using namespace std;

namespace kanan {
	FighterNoForce::FighterNoForce()
		: PatchMod{ "Fighter No Force", "While in hide interface mode, using chain combo fighter skills will turn the interface back on. This disables this trigger, persisting the interface to stay hidden." }
	{
		addPatch("83 79 44 01 75 09 6A 00 6A 00 E8", 4, { 0xEB });
		addPatch("83 79 44 01 75 09 6A 00 6A 00 E8", 4, { 0xEB });
	}
}
