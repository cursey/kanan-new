#include "CancelableLanceCounter.hpp"

using namespace std;

namespace kanan {
	CancelableLanceCounter::CancelableLanceCounter()
		: PatchMod{ "Cancelable Lance Counter", "Allows you to cancel lance counter once loaded." }
	{
		addPatch("55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 83 EC 0C 53 56 57 A1 ? ? ? ? 33 C5 50 8D 45 F4 64 A3 ? ? ? ? E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? E8", 0, { 0xB0, 0x01, 0xC2, 0x04, 0x00 });
	}
}
