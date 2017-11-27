#include "MinuteClock.hpp"

using namespace std;

namespace kanan {
	MinuteClock::MinuteClock()
		: PatchMod{ "Minute Clock", "Updates the clock on a 1 minute basis as opposed to 10 minutes." }
	{
		addPatch("89 45 C4 39 9F", 0, { 0x90, 0x90, 0x90 });
		addPatch("89 4D C4 39 9F", 0, { 0x90, 0x90, 0x90 });
	}
}
