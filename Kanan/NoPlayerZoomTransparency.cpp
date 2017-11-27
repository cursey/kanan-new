#include "NoPlayerZoomTransparency.hpp"

using namespace std;

namespace kanan {
	NoPlayerZoomTransparency::NoPlayerZoomTransparency()
		: PatchMod{ "No Player Zoom Transparency", "Stops players from becoming translucent while colliding the camera with other players." }
	{
		addPatch("4E F7 DE 1B F6 81 E6 FE 07 00 00", 6, { 0xC7, 0xF0, 0x0F });
		addPatch("74 17 53 57 6a 02 57", 5, { 0x05 }); 
	}
}
