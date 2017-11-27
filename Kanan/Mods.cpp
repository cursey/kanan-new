#include "BitmapFont.hpp"
#include "ShowCombatPower.hpp"
#include "ElfLagFix.hpp"
#include "WindowsAppearFaster.hpp"
#include "OneClickRevive.hpp"
#include "NoMountTimeout.hpp"
#include "RangedAttackSwap.hpp"
#include "FreeZoom.hpp"

#include "AutoSetMTU.hpp"
#include "DisableNagle.hpp"
#include "BorderlessWindow.hpp"
#include "EnableMultiClient.hpp"
#include "EntityViewer.hpp"
#include "SimpleToggleMod.hpp"


#include "LogoutInConversation.hpp"
#include "MoveToSameChannel.hpp"
#include "NoCensorship.hpp"
#include "NoChannelMoveDenial.hpp"
#include "NoChannelMoveDescription.hpp"
#include "NoLogoutPenaltyMsg.hpp"
#include "NoSkillRankupWindow.hpp"
#include "PartyNoClose.hpp"
#include "RemoveDungeonFog.hpp"
#include "RemoveScreenShake.hpp"

#include "Log.hpp"
#include "Mods.hpp"

using namespace std;

namespace kanan {
    Mods::Mods()
        : m_mods{}
    {
        log("Entering Mods cosntructor.");

        m_mods.emplace_back(make_unique<BitmapFont>());
        m_mods.emplace_back(make_unique<ShowCombatPower>());
        m_mods.emplace_back(make_unique<ElfLagFix>());
        m_mods.emplace_back(make_unique<WindowsAppearFaster>());
        m_mods.emplace_back(make_unique<OneClickRevive>());
        m_mods.emplace_back(make_unique<NoMountTimeout>());
        m_mods.emplace_back(make_unique<LogoutInConversation>());
        m_mods.emplace_back(make_unique<MoveToSameChannel>());
        m_mods.emplace_back(make_unique<NoCensorship>());
        m_mods.emplace_back(make_unique<NoChannelMoveDenial>());
        m_mods.emplace_back(make_unique<NoChannelMoveDescription>());
        m_mods.emplace_back(make_unique<NoLogoutPenaltyMsg>());
        m_mods.emplace_back(make_unique<NoSkillRankupWindow>());
        m_mods.emplace_back(make_unique<PartyNoClose>());
        m_mods.emplace_back(make_unique<RemoveDungeonFog>());
        m_mods.emplace_back(make_unique<RemoveScreenShake>());
        m_mods.emplace_back(make_unique<FreeZoom>());
        m_mods.emplace_back(make_unique<RangedAttackSwap>());

        m_mods.emplace_back(make_unique<AutoSetMTU>());
        m_mods.emplace_back(make_unique<DisableNagle>());
        m_mods.emplace_back(make_unique<BorderlessWindow>());
        m_mods.emplace_back(make_unique<EnableMultiClient>());
        m_mods.emplace_back(make_unique<EntityViewer>());

        log("Leaving Mods constructor.");
    }
}
