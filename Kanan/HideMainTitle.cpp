#include "HideMainTitle.hpp"

using namespace std;

namespace kanan {
	HideMainTitle::HideMainTitle()
		: PatchMod{ "Hide Main Title", "Hide away main titles (Blood Master, Meteor Strike Master, etc) of all players, pets and enemies. Note this also includes Ancient enemies." }
	{
		addPatch("08 00 66 8b 41 10 C3", 2, { 0x31, 0xC0, 0x90, 0x90 });
	}
}
