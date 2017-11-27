#include "FixPetNoteCursor.hpp"

using namespace std;

namespace kanan {
	FixPetNoteCursor::FixPetNoteCursor()
		: PatchMod{ "Fix Pet Note Cursor", "Causes the cursor to jump to the end when editing a pet note, allowing you to erase what was previously written." }
	{
		addPatch("C7 45 08 00 00 00 00 FF 86 10 01 00 00", 7, { 0x01, 0xAE });
	}
}
