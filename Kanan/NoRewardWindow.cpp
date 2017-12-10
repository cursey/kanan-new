#include "NoRewardWindow.hpp"

using namespace std;

namespace kanan {
	NoRewardWindow::NoRewardWindow()
		: PatchMod{ "No Reward Window", "Removes the \"Obtained X item\" reward window that shows up when acquiring items, stats or exp." }
	{
		addPatch("3B D6 0F 85 ? ? ? ? 33 DB", 0, { 0x39, 0xDB, 0x0f, 0x84 });
	}
}
