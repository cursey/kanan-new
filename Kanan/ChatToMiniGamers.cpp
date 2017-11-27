#include "ChatToMiniGamers.hpp"

using namespace std;

namespace kanan {
	ChatToMiniGamers::ChatToMiniGamers()
		: PatchMod{ "Chat To Mini-gamers", "Allow you to start a chat with friends who have their status set to \"Mini - game\"." }
	{
		addPatch("75 13 8D 4D 08 56", 0, { 0xEB });
	}
}
