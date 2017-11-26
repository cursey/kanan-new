#include "NoChannelMoveDenial.hpp"

namespace kanan {
	NoChannelMoveDenial::NoChannelMoveDenial() : SimpleToggleMod("NoChannelMoveDenial", "No Channel Move Denial", "Allows you change channel while in conversation with a NPC.")
	{
		addPatch("0F 84 ? ? ? ? 89 5D F0 89 5D FC 8B CE", 0, { 0x90, 0xE9 }); 
	}
}