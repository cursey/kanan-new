#include <imgui.h>
#include <Scan.hpp>
#include <Utility.hpp>

#include "Kanan.hpp"
#include "Log.hpp"

#include "AutoChangeChannels.hpp"

namespace kanan {
static AutoChangeChannels* g_auto_cc{};
AutoChangeChannels::AutoChangeChannels() {
    log("[AutoChangeChannels] Entering constructor");

    auto call_something = scan("client.exe", "E8 ? ? ? ? C6 45 FC ? 8D 45 A4 89 45 14");

    if (!call_something) {
        log("[AutoChangeChannels] Failed to find call_something address!");
        return;
    } else {
        log("[AutoChangeChannels] Found call_something @ %p", *call_something);
    }

    auto call_add_condition = scan("client.exe", rel_to_abs(*call_something + 1), "E8 ? ? ? ? 8B 08");

    if (!call_add_condition) {
        log("[AutoChangeChannels] Failed to find call_add_condition address!");
        return;
    } else {
        log("[AutoChangeChannels] Found call_add_condition @ %p", *call_add_condition);
    }

    auto add_condition = rel_to_abs(*call_add_condition + 1);

    log("[AutoChangeChannels] Found add_condition @ %p", add_condition);

    m_add_condition_hook =
        std::make_unique<FunctionHook>(add_condition, (uintptr_t)&AutoChangeChannels::hooked_add_condition);

    log("[AutoChangeChannels] Hooked add_condition");

	g_auto_cc = this;

	log("[AutoChangeChannels] Leaving constructor");

}

void AutoChangeChannels::onFrame() {
    if (!m_is_enabled) {
        return;
    }

    // Get the local player.
    auto game = g_kanan->getGame();
    auto me = game->getLocalCharacter();

    if (me == nullptr) {
        return;
    }

    auto me_id = me->getID();

    if (!me_id) {
        return;
    }

    // Loop through all the characters and see if any NPC is targetting us.
    auto ents = game->getEntityList();

    if (ents == nullptr) {
        return;
    }

    auto& characters = ents->characters;
    auto node = characters.root;

    for (uint32_t i = 0; i <= characters.count && node != nullptr; ++i, node = node->next) {
        auto character = (KCharacter*)node->entry->character;

        if (character == nullptr) {
            continue;
        }

        auto id = character->getID();

        if (!id || *id < 0x10'F000'0000'0000) {
            continue;
        } 

        // If this NPC is targetting us, change channel.
        if (character->targetID == *me_id) {
            game->changeChannel(g_auto_cc->m_channel);
            return;
        }
    }
}

void AutoChangeChannels::onUI() {
    if (ImGui::CollapsingHeader("Auto-Change Channel")) {
		ImGui::Text("Enabling this makes you change channels whenever you get spiked, fairied, smash bled, or a monster targets you.");
		ImGui::Checkbox("Enable Auto-Change Channels", &m_is_enabled);
		ImGui::SliderInt("Channel", &m_channel, 1, 10);

		if (ImGui::Button("Change Channel Immediately")) {
			auto game = g_kanan->getGame();
			auto localCharacter = game->getLocalCharacter();

			if (localCharacter != nullptr) {
				game->changeChannel(m_channel);
			}
		}
	}
}

void AutoChangeChannels::onConfigLoad(const Config& cfg) {
    m_is_enabled = cfg.get<bool>("AutoChangeChannels.Enabled").value_or(false);
}

void AutoChangeChannels::onConfigSave(Config& cfg) {
    cfg.set<bool>("AutoChangeChannels.Enabled", m_is_enabled);
}

int __fastcall AutoChangeChannels::hooked_add_condition(CCharacter::CConditionMgr::CCondition* condition, uint32_t EDX,
    int a2, char a3, int a4, int a5, ConditionInfo* info) {

	if (g_auto_cc->m_is_enabled && info != nullptr) {
        // Note: you can find the IDs in data/db/characondition.xml if they ever change or more need
        // to be added.
        switch (info->id) {
        case 360: // Fairy
        case 317: // Smash bleed
        case 296: // Celestial spike
        {
            auto game = g_kanan->getGame();
            auto me = game->getLocalCharacter();

            if (me != nullptr) {
                auto condition_mgr = me->condition;

                if (condition_mgr != nullptr && condition == condition_mgr->condition) {
                    game->changeChannel(g_auto_cc->m_channel);
                }
            }
        }
        }
    }

    auto orig = (decltype(hooked_add_condition)*)g_auto_cc->m_add_condition_hook->getOriginal();
    return orig(condition, EDX, a2, a3, a4, a5, info);

}
} // namespace kanan

