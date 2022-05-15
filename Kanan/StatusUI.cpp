#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

#include "Kanan.hpp"
#include "StatusUI.hpp"

namespace kanan {
    StatusUI::StatusUI()
        : m_isShowing{},
        m_showChainBladeStuff{},
        m_hp{},
        m_mp{},
        m_sp{},
        m_dorcha{},
        m_tuairim{}
    {
        m_hp.reserve(100);
        m_mp.reserve(100);
        m_sp.reserve(100);
        m_dorcha.reserve(100);
        m_tuairim.reserve(100);
    }

    void StatusUI::onFrame() {
        if (!m_isShowing) {
            return;
        }

        ImGui::SetNextWindowSize(ImVec2{ 400.0f, 200.0f }, ImGuiCond_FirstUseEver);

        // Give the window a different style when the UI is closed indicating that
        // the status window can't be interacted with.
        if (g_kanan->isUIOpen()) {
            if (!ImGui::Begin("Status", &m_isShowing, ImGuiWindowFlags_NoCollapse)) {
                ImGui::End();
                return;
            }
        }
        else {
            if (!ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
                ImGui::End();
                return;
            }
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
        auto hpRatio = fdiv(hp, maxHP);

        auto mp = param->mana.value;
        auto maxMP = param->manaMaxBase.value + param->manaMaxMod.value;
        auto mpRatio = fdiv(mp, maxMP);

        auto sp = param->stamina.value;
        auto maxSP = param->staminaMaxBase.value + param->staminaMaxMod.value;
        auto spRatio = fdiv(sp, maxSP);

        sprintf_s(m_hp.data(), m_hp.capacity(), "HP: %.02f/%.02f", hp, maxHP);
        sprintf_s(m_mp.data(), m_mp.capacity(), "MP: %.02f/%.02f", mp, maxMP);
        sprintf_s(m_sp.data(), m_sp.capacity(), "SP: %.02f/%.02f", sp, maxSP);

        // Colors taken from Mabi's default UI in ABGR format.
        progressBar(hpRatio, ImVec2{ -1.0f, 0.0f }, 0xFF9140CF, m_hp.c_str());
        progressBar(mpRatio, ImVec2{ -1.0f, 0.0f }, 0xFFB66C5B, m_mp.c_str());
        progressBar(spRatio, ImVec2{ -1.0f, 0.0f }, 0xFF20B9E9, m_sp.c_str());

        if (m_showChainBladeStuff) {
            auto dorcha = param->dorcha.value;
            auto maxDorcha = param->dorchaMaxBase.value;
            auto dorchaRatio = fdiv(dorcha, maxDorcha);

            auto tuairim = param->tuairim.value;
            auto maxTuairim = param->tuairimMaxBase.value;
            auto tuairimRatio = fdiv(tuairim, maxTuairim);

            sprintf_s(m_dorcha.data(), m_dorcha.capacity(), "Dorcha: %.02f/%.02f", dorcha, maxDorcha);
            sprintf_s(m_tuairim.data(), m_tuairim.capacity(), "Bachram Boost: %.02f%%", tuairimRatio * 100.0f);

            progressBar(dorchaRatio, ImVec2{ -1.0f, 0.0f }, 0xFF6F00A4, m_dorcha.c_str());
            progressBar(tuairimRatio, ImVec2{ -1.0f, 0.0f }, 0xFFCFABA1, m_tuairim.c_str());
        }

        ImGui::End();
    }

    void StatusUI::onUI() {
        if (ImGui::CollapsingHeader("Status UI")) {
            ImGui::TextWrapped("Creates a window with larger progress bars displaying your characters status (HP/MP/Stam)");
            ImGui::Checkbox("Show Status UI", &m_isShowing);
            ImGui::Checkbox("Show Chain Blade Status", &m_showChainBladeStuff);
        }
    }

    void StatusUI::onConfigLoad(const Config& cfg) {
        m_isShowing = cfg.get<bool>("StatusUI.Showing").value_or(false);
        m_showChainBladeStuff = cfg.get<bool>("StatusUI.ShowChainBlade").value_or(false);
    }
    
    void StatusUI::onConfigSave(Config& cfg) {
        cfg.set<bool>("StatusUI.Showing", m_isShowing);
        cfg.set<bool>("StatusUI.ShowChainBlade", m_showChainBladeStuff);
    }

    // Note: this is copied from ImGui::ProgressBar altered just to let the caller
    // specify a color for the progress bar.
    void StatusUI::progressBar(float fraction, const ImVec2& sizeArg, ImU32 color, const char* overlay) {
        using namespace ImGui;

        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        ImVec2 pos = window->DC.CursorPos;
        ImRect bb(pos, pos + CalcItemSize(sizeArg, CalcItemWidth(), g.FontSize + style.FramePadding.y*2.0f));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, 0))
            return;

        // Render
        fraction = ImSaturate(fraction);
        RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
        bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));
        const ImVec2 fill_br = ImVec2(ImLerp(bb.Min.x, bb.Max.x, fraction), bb.Max.y);
        RenderRectFilledRangeH(window->DrawList, bb, GetColorU32(color), 0.0f, fraction, style.FrameRounding);

        // Default displaying the fraction as percentage string, but user can override it
        char overlay_buf[32];
        if (!overlay)
        {
            ImFormatString(overlay_buf, IM_ARRAYSIZE(overlay_buf), "%.0f%%", fraction * 100 + 0.01f);
            overlay = overlay_buf;
        }

        ImVec2 overlay_size = CalcTextSize(overlay, NULL);
        if (overlay_size.x > 0.0f)
            RenderTextClipped(ImVec2(ImClamp(fill_br.x + style.ItemSpacing.x, bb.Min.x, bb.Max.x - overlay_size.x - style.ItemInnerSpacing.x), bb.Min.y), bb.Max, overlay, NULL, &overlay_size, ImVec2(0.0f, 0.5f), &bb);
    }

    float StatusUI::fdiv(float x, float y) {
        if (y == 0.0f) {
            return 0.0f;
        }

        return x / y;
    }
}
