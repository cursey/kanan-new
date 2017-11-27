#include "MuteCommerceImp.hpp"

using namespace std;

namespace kanan {
	MuteCommerceImp::MuteCommerceImp()
		: PatchMod{ "Mute Commerce Imp", "Disables chatting notifications from the trade imp while commercing. Will still trigger bandit alert." }
	{
		addPatch("8B ? ? 03 ? ? 3B ? 08 0F 83", 9, { 0x90, 0xE9 });
	}
}
