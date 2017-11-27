#include "FreeIndoorCamera.hpp"

using namespace std;

namespace kanan {
	FreeIndoorCamera::FreeIndoorCamera()
		: PatchMod{ "Free Indoor Camera", "Allows you to freely rotate the camera while indoors." }
	{
		addPatch("57 8b 7d 08 0f 84 22 02 00 00", 4, { 0x90, 0xE9 });
	}
}
