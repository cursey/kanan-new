#include <Scan.hpp>
#include <Utility.hpp>
#include <imgui.h>

#include "Kanan.hpp"
#include "Log.hpp"

#include "AutoChangeChannels.hpp"

namespace kanan {
    //custom bool for checkbox
    bool enableNumbcc = false;


    static AutoChangeChannels* g_auto_cc{};
    AutoChangeChannels::AutoChangeChannels() {
        log("[AutoChangeChannels] Entering constructor");

        auto call_something = scan("client.exe", "E8 ? ? ? ? C6 45 FC ? 8D 45 A4 89 45 14");

        if (!call_something) {
            log("[AutoChangeChannels] Failed to find call_something address!");
            return;
        }
        else {
            log("[AutoChangeChannels] Found call_something @ %p", *call_something);
        }

        auto call_add_condition = scan("client.exe", rel_to_abs(*call_something + 1), "E8 ? ? ? ? 8B 08");

        if (!call_add_condition) {
            log("[AutoChangeChannels] Failed to find call_add_condition address!");
            return;
        }
        else {
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
    // poi:
    // poorly made cc code for ccing on numpad press, this could be improved but i havent a clue how at my current skill level

    void numpadCC() {
        auto game = g_kanan->getGame();
        auto me = game->getLocalCharacter();
        int wantedCh{ 1 };

        if (me == nullptr) {
            return;
        }
        else {
            if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
                if (enableNumbcc) {
                    wantedCh = 1;
                    log("numbpad 1 pressed, attemping cc to ch1");
                    game->changeChannel(wantedCh);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
                if (enableNumbcc) {
                    wantedCh = 2;
                    log("numbpad 2 pressed, attemping cc to ch2");
                    game->changeChannel(wantedCh);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
                if (enableNumbcc) {
                    wantedCh = 3;
                    log("numbpad 3 pressed, attemping cc to ch3");
                    game->changeChannel(wantedCh);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD4) & 1) {
                if (enableNumbcc) {
                    wantedCh = 4;
                    log("numbpad 4 pressed, attemping cc to ch4");
                    game->changeChannel(wantedCh);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD5) & 1) {
                if (enableNumbcc) {
                    wantedCh = 5;
                    log("numbpad 5 pressed, attemping cc to ch5");
                    game->changeChannel(wantedCh);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD6) & 1) {
                if (enableNumbcc) {
                    wantedCh = 6;
                    log("numbpad 6 pressed, attemping cc to ch6");
                    game->changeChannel(wantedCh);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD7) & 1) {
                if (enableNumbcc) {
                    wantedCh = 7;
                    log("numbpad 7 pressed, attemping cc to ch7");
                    game->changeChannel(wantedCh);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD8) & 1) {
                if (enableNumbcc) {
                    wantedCh = 8;
                    log("numbpad 8 pressed, attemping cc to ch8");
                    game->changeChannel(wantedCh);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD9) & 1) {
                if (enableNumbcc) {
                    wantedCh = 9;
                    log("numbpad 9 pressed, attemping cc to ch9");
                    game->changeChannel(wantedCh);
                }
            }
            if (GetAsyncKeyState(VK_NUMPAD0) & 1) {
                if (enableNumbcc) {
                    wantedCh = 10;
                    log("numbpad 0 pressed, attemping cc to ch10");
                    game->changeChannel(wantedCh);
                }
            }

        }



    }

    void AutoChangeChannels::onFrame() {
        //custom code for attemping to cc
        numpadCC();


        if (!m_is_enabled || !m_cc_when_targeted) {
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
            ImGui::TextWrapped("Enabling this makes you change channels whenever you get spiked, fairied, smash bled, or "
                "(optionally) a monster targets you.");
            ImGui::Checkbox("Enable Auto-Change Channels", &m_is_enabled);
            ImGui::Checkbox("Change Channels When Targeted by Monsters", &m_cc_when_targeted);
            ImGui::SliderInt("Channel", &m_channel, 1, 10);

            //numpad to cc patch
            ImGui::Checkbox("use numpad to cc", &enableNumbcc);

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
        m_cc_when_targeted = cfg.get<bool>("AutoChangeChannels.WhenTargeted").value_or(false);
    }

    void AutoChangeChannels::onConfigSave(Config& cfg) {
        cfg.set<bool>("AutoChangeChannels.Enabled", m_is_enabled);
        cfg.set<bool>("AutoChangeChannels.WhenTargetted", m_cc_when_targeted);
        cfg.set<bool>("AutoChangeChannels.enableNumbcc", enableNumbcc);

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
