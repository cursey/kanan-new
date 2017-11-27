#include "IgnoreMaintainFrame.hpp"

using namespace std;

namespace kanan {
	IgnoreMaintainFrame::IgnoreMaintainFrame()
		: PatchMod{ "Ignore Maintain Frame", "Ignore the maintain frame setting during character minimization." }
	{
		addPatch("89 4E 50 8B 57 54 89 56 54 0F B6 47 58 88 46 58 0F B6 4F 59", 1, { 0x6E });
	}
}
