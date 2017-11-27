#include "ClientSideDevCat.hpp"

using namespace std;

namespace kanan {
	ClientSideDevCat::ClientSideDevCat()
		: PatchMod{ "Client Side DevCat", "Enables client-sided effects provided with the devCAT title including visible HP values under HP bars." }
	{
		addPatch("74 21 8B 16 8B 82 94 00 00 00 8B CE", 0, { 0xEB });
	}
}
