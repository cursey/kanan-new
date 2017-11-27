#include "SecondTimer.hpp"

using namespace std;

namespace kanan {
	SecondTimer::SecondTimer()
		: PatchMod{ "Second Timer", "Update the time elapsed timer in theatre/shadow missions every 1 second as opposed to every 15 seconds." }
	{
		addPatch("B8 ? ? ? ? F7 E6 C1 EA 03 42", 0, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }); //18 nops
		addPatch("83 F8 0F 0F 97 C0", 2, { 0x00 });
		addPatch("6A 0F 83 DE 00", 1, { 0x01 });
		addPatch("C1 E6 04 68 ? ? ? ? 8D 4D EC", 0, { 0x90, 0x90, 0x90 });
		addPatch("C1 E6 04 68 ? ? ? ? 8D 4D EC", 1, { 0x90, 0x90 });
	}
}
