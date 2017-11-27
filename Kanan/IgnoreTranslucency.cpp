#include "IgnoreTranslucency.hpp"

using namespace std;

namespace kanan {
	IgnoreTranslucency::IgnoreTranslucency()
		: PatchMod{ "Ignore Translucency", "Stops objects from becoming translucent while colliding the camera with other objects." }
	{
		addPatch("39 70 04 75 39 3B FE", 3, { 0x90, 0x90});
	}
}
