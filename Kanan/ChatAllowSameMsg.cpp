#include "ChatAllowSameMsg.hpp"

using namespace std;

namespace kanan {
	ChatAllowSameMsg::ChatAllowSameMsg()
		: PatchMod{ "Allow Same Chat Messages", "Bypass \"Skipped repeated messages for network stability.\" error message while chatting." }
	{
		addPatch("0F 85 ? ? ? ? 33 FF C6 45 F3 01", 0, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		addPatch("0F 85 ? ? ? ? 33 FF 80 7D 10 00", 0, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
	}
}
