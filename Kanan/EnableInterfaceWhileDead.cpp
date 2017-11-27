#include "EnableInterfaceWhileDead.hpp"

using namespace std;

namespace kanan {
	EnableInterfaceWhileDead::EnableInterfaceWhileDead()
		: PatchMod{ "Enable Interface While Dead", "Take full control of the interface while in unconcious state." }
	{
		addPatch("FF 87 ? ? ? ? EB 11", 0, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
	}
}
