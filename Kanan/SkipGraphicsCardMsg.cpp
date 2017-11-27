#include "SkipGraphicsCardMsg.hpp"

using namespace std;

namespace kanan {
	SkipGraphicsCardMsg::SkipGraphicsCardMsg()
		: PatchMod{ "Skip Graphics Card Message", "Automatically skips the \"The graphic card installed on your system does not ensure proper execution of the Mabinogi client.\" when launching the client." }
	{
		addPatch("83 C4 08 84 C0 75 7F 8D", 5, { 0xEB });
	}
}
