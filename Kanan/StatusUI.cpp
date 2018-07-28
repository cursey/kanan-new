#include <imgui.h>

#include "Kanan.hpp"
#include "StatusUI.hpp"

namespace kanan {
    StatusUI::StatusUI()
        : m_isShowing{},
        m_hp{},
        m_mp{},
        m_sp{}
    {
        m_hp.reserve(100);
        m_mp.reserve(100);
        m_sp.reserve(100);
    }

    void StatusUI::onFrame() {
        if (!m_isShowing) {
            return;
        }

        ImGui::SetNextWindowSize(ImVec2{ 400.0f, 200.0f }, ImGuiSetCond_FirstUseEver);

        if (!ImGui::Begin("Status", &m_isShowing)) {
            ImGui::End();
            return;
        }

        auto game = g_kanan->getGame();
        auto localCharacter = game->getLocalCharacter();

        if (localCharacter == nullptr) {
            ImGui::TextWrapped("Local character not found!");
            ImGui::End();
            return;
        }

        auto param = localCharacter->parameter;

        if (param == nullptr) {
            ImGui::TextWrapped("Local character's status not found!");
            ImGui::End();
            return;
        }

        auto hp = param->life.value;
        auto maxHP = param->lifeMaxBase.value + param->lifeMaxMod.value;
        auto hpRatio = hp / maxHP;

        auto mp = param->mana.value;
        auto maxMP = param->manaMaxBase.value + param->manaMaxMod.value;
        auto mpRatio = mp / maxMP;

        auto sp = param->stamina.value;
        auto maxSP = param->staminaMaxBase.value + param->staminaMaxMod.value;
        auto spRatio = sp / maxSP;


        sprintf_s(m_hp.data(), m_hp.capacity(), "HP: %.02f/%.02f", hp, maxHP);
        sprintf_s(m_mp.data(), m_mp.capacity(), "MP: %.02f/%.02f", mp, maxMP);
        sprintf_s(m_sp.data(), m_sp.capacity(), "SP: %.02f/%.02f", sp, maxSP);

        ImGui::ProgressBar(hpRatio, ImVec2{ -1.0f, 0.0f }, m_hp.c_str());
        ImGui::ProgressBar(mpRatio, ImVec2{ -1.0f, 0.0f }, m_mp.c_str());
        ImGui::ProgressBar(spRatio, ImVec2{ -1.0f, 0.0f }, m_sp.c_str());

        ImGui::End();
    }

    void StatusUI::onUI() {
        if (ImGui::CollapsingHeader("Status UI")) {
            ImGui::TextWrapped("Creates a window with larger progress bars displaying your characters status (HP/MP/Stam)");
            ImGui::Checkbox("Show Status UI", &m_isShowing);
        }
    }

    void StatusUI::onConfigLoad(const Config& cfg) {
        m_isShowing = cfg.get<bool>("StatusUI.Showing").value_or(false);
    }
    
    void StatusUI::onConfigSave(Config& cfg) {
        cfg.set<bool>("StatusUI.Showing", m_isShowing);
    }
}
