#include "EnableFSAA.hpp"

using namespace std;

namespace kanan {
	EnableFSAA::EnableFSAA()
		: PatchMod{ "Enable FSAA", "Enables the option to turn on Full-screen Anti-aliasing in Mabinogi's Options menu." }
	{
		addPatch("53 FF D2 8B 0D ? ? ? ? 6A 0C E8 ? ? ? ? 68 ? ? ? ? 89 45 D0", 0, { 0x90, 0x90, 0x90 });
	}
}
