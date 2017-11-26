#include "RemoveScreenShake.hpp"

namespace kanan {
	RemoveScreenShake::RemoveScreenShake() : SimpleToggleMod("RemoveScreenShake", "Remove Screen Shake", "Stops the screen from shaking when using Fireball, summoning a dragon, etc.")
	{
		addPatch("D9 01 8B 46 64", 0, { 0xD9, 0xEE });
		addPatch("D9 01 8B 46 64", 9, { 0xD9, 0xEE, 0x90 });
		addPatch("D9 01 8B 46 64", 15, { 0xD9, 0xEE, 0x90 });
		addPatch("D9 01 8B 46 64", 21, { 0xD9, 0xEE, 0x90 });
	}
}