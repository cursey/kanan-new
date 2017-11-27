#include "LogoutInConversation.hpp"

namespace kanan {
	LogoutInConversation::LogoutInConversation() : SimpleToggleMod("LogoutInConversation", "Logout in Conversation", "Allows you to logout in the middle of a conversation with a NPC.")
	{
		addPatch("84 C0 74 19 B8 ? ? ? ? 8B 4D F4 64 89 0D ? ? ? ? 59 5F 5E 5B 8B E5 5D C2 04 00", 2, { 0xEB });
	}
}