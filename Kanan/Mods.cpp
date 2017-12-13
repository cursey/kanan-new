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
#include "AlwaysTransCollectMode.hpp"
#include "CancelableLanceCounter.hpp"
#include "ChatAllowSameMsg.hpp"
#include "ChatNoRateLimit.hpp"
#include "ChatToMiniGamers.hpp"
#include "ClientSideDevCat.hpp"
#include "DisableScreenFlash.hpp"
#include "DungeonMapResize.hpp"
#include "EnableFSAA.hpp"
#include "EnableInterfaceWhileDead.hpp"
#include "EnableMinimapZoom.hpp"
#include "FarAltText.hpp"
#include "FighterNoForce.hpp"
#include "FixPetNoteCursor.hpp"
#include "FreeIndoorCamera.hpp"
#include "HideMainTitle.hpp"
#include "HideNPCCurtains.hpp"
#include "HideObjects.hpp"
#include "HideSecondTitle.hpp"
#include "HotkeyAnyItem.hpp"
#include "IgnoreMaintainFrame.hpp"
#include "IgnoreTranslucency.hpp"
#include "MiniTitleMenu.hpp"
#include "MinuteClock.hpp"
#include "MuteCommerceImp.hpp"
#include "NPCFastText.hpp"
#include "NoPersonalShopCreationMsg.hpp"
#include "NoPlayerZoomTransparency.hpp"
#include "NoRenderSky.hpp"
#include "SecondTimer.hpp"
#include "SelfRightClickable.hpp"
#include "ShowDeadlyHP.hpp"
#include "ShowItemClass.hpp"
#include "ShowItemTradePrice.hpp"
#include "SkipGraphicsCardMsg.hpp"
#include "TalkToEgo.hpp"
#include "ViewNPCEquipment.hpp"
#include "ColorAltText.hpp"
#include "ZeroFogDistance.hpp"
#include "NoRewardWindow.hpp"

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
        m_mods.emplace_back(make_unique<FreeZoom>());
        m_mods.emplace_back(make_unique<RangedAttackSwap>());
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
		m_mods.emplace_back(make_unique<AlwaysTransCollectMode>());
		m_mods.emplace_back(make_unique<CancelableLanceCounter>());
		m_mods.emplace_back(make_unique<ChatAllowSameMsg>());
		m_mods.emplace_back(make_unique<ChatNoRateLimit>());
		m_mods.emplace_back(make_unique<ChatToMiniGamers>());
		m_mods.emplace_back(make_unique<ClientSideDevCat>());
		m_mods.emplace_back(make_unique<DisableScreenFlash>());
		m_mods.emplace_back(make_unique<DungeonMapResize>());
		m_mods.emplace_back(make_unique<EnableFSAA>());
		m_mods.emplace_back(make_unique<EnableInterfaceWhileDead>());
		m_mods.emplace_back(make_unique<EnableMinimapZoom>());
		m_mods.emplace_back(make_unique<FarAltText>());
		m_mods.emplace_back(make_unique<FighterNoForce>());
		m_mods.emplace_back(make_unique<FixPetNoteCursor>());
		m_mods.emplace_back(make_unique<FreeIndoorCamera>());
		m_mods.emplace_back(make_unique<HideMainTitle>());
		m_mods.emplace_back(make_unique<HideNPCCurtains>());
		m_mods.emplace_back(make_unique<HideObjects>());
		m_mods.emplace_back(make_unique<HideSecondTitle>());
		m_mods.emplace_back(make_unique<HotkeyAnyItem>());
		m_mods.emplace_back(make_unique<IgnoreMaintainFrame>());
		m_mods.emplace_back(make_unique<IgnoreTranslucency>());
		m_mods.emplace_back(make_unique<MiniTitleMenu>());
		m_mods.emplace_back(make_unique<MinuteClock>());
		m_mods.emplace_back(make_unique<MuteCommerceImp>());
		m_mods.emplace_back(make_unique<NPCFastText>());
		m_mods.emplace_back(make_unique<NoPersonalShopCreationMsg>());
		m_mods.emplace_back(make_unique<NoPlayerZoomTransparency>());
		m_mods.emplace_back(make_unique<NoRenderSky>());
		m_mods.emplace_back(make_unique<SecondTimer>());
		m_mods.emplace_back(make_unique<SelfRightClickable>());
		m_mods.emplace_back(make_unique<ShowDeadlyHP>());
		m_mods.emplace_back(make_unique<ShowItemClass>());
		m_mods.emplace_back(make_unique<ShowItemTradePrice>());
		m_mods.emplace_back(make_unique<SkipGraphicsCardMsg>());
		m_mods.emplace_back(make_unique<TalkToEgo>());
		m_mods.emplace_back(make_unique<ViewNPCEquipment>());
		m_mods.emplace_back(make_unique<ColorAltText>());
		m_mods.emplace_back(make_unique<ZeroFogDistance>());
		m_mods.emplace_back(make_unique<NoRewardWindow>());

        m_mods.emplace_back(make_unique<AutoSetMTU>());
        m_mods.emplace_back(make_unique<DisableNagle>());
        m_mods.emplace_back(make_unique<BorderlessWindow>());
        m_mods.emplace_back(make_unique<EnableMultiClient>());
        m_mods.emplace_back(make_unique<EntityViewer>());

        log("Leaving Mods constructor.");
    }
}
