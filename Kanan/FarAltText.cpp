#include "FarAltText.hpp"

using namespace std;

namespace kanan {
	FarAltText::FarAltText()
		: PatchMod{ "Far Alt Text", "Allows you to see entities while holding down ALT from much further away." }
	{
		addPatch("0F 85 ? ? ? ? D9 EE D9 45 D4", 0, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
	}
}
