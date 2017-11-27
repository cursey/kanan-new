#include "EnableMinimapZoom.hpp"

using namespace std;

namespace kanan {
	EnableMinimapZoom::EnableMinimapZoom()
		: PatchMod{ "Enable Minimap Zoom", "Allows zooming out on all minimaps instead of just some of them." }
	{
		addPatch("89 4E 74 80 78 40 00 74 16", 1, { 0x6E });
	}
}
