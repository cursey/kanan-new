#include "MoveToSameChannel.hpp"

namespace kanan {
	MoveToSameChannel::MoveToSameChannel() : SimpleToggleMod("MoveToSameChannel", "Move To the Same Channel", "Allows you to move to the same channel.")
	{
		addPatch("0F 84 ?? ?? ?? ?? 68 ?? ?? ?? ?? 8D 4D E0 E8 ?? ?? ?? ?? C6 45 FC 04 8D 55 E0 52 8D 45 B8", 0, { 0x90, 0xE9 });
	}
}