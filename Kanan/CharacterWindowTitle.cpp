#include <imgui.h>

#include "CharacterWindowTitle.hpp"
#include "KCharacter.hpp"
#include "Kanan.hpp"
#include "Log.hpp"

namespace kanan {
CharacterWindowTitle::CharacterWindowTitle() : m_is_enabled{}, m_currentCharTitle{} {
}

void CharacterWindowTitle::onFrame() {

    // if not enabled return
    if (!m_is_enabled) {
        if (m_currentCharTitle == "Mabinogi") {
            return;
        } else {
            m_currentCharTitle = "Mabinogi";
            SetWindowTextA(GetActiveWindow(), "Mabinogi");
        }
        return;
    }
    // get local char
    auto game = g_kanan->getGame();
    auto localCharacter = game->getLocalCharacter();

    // if local char doesnt exist return
    if (localCharacter == nullptr) {

        return;
    }
    // get local char name
    auto localCharacterName = localCharacter->getName();

    // if title is == to local char name do nothing
    if (m_currentCharTitle == localCharacterName->c_str()) {
        return;
    }
    // else update
    else {
        SetWindowTextA(GetActiveWindow(), localCharacterName->c_str());

        m_currentCharTitle = localCharacterName->c_str();
        log("Updated title to %s", localCharacterName->c_str());
    }
}

void CharacterWindowTitle::onUI() {
    if (ImGui::CollapsingHeader("Character name in Title Bar")) {
        ImGui::TextWrapped("Replaces Mabinogi with the name of the currently logged in character in the client window title bar.");
        ImGui::Checkbox("Enable", &m_is_enabled);
    }
}

void CharacterWindowTitle::onConfigLoad(const Config& cfg) {
    m_is_enabled = cfg.get<bool>("CharacterWindowTitle.Showing").value_or(false);
}

void CharacterWindowTitle::onConfigSave(Config& cfg) {
    cfg.set<bool>("CharacterWindowTitle.Showing", m_is_enabled);
}
} // namespace kanan