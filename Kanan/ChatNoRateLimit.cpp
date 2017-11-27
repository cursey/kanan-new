#include "ChatNoRateLimit.hpp"

using namespace std;

namespace kanan {
	ChatNoRateLimit::ChatNoRateLimit()
		: PatchMod{ "Chat No Rate Limit", "Bypass \"You cannot enter multiple messages so quickly.Please wait a moment and try again.\" error message while chatting." }
	{
		addPatch("76 B0 68 ? ? ? ? 8D 4D DC", 0, { 0xEB });
	}
}
