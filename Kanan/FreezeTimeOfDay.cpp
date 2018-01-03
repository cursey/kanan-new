#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "FreezeTimeOfDay.hpp"

using namespace std;

namespace kanan {
    static FreezeTimeOfDay* g_freezeTimeOfDay{ nullptr };

    FreezeTimeOfDay::FreezeTimeOfDay()
        : m_isEnabled{ false },
        m_timeOfDay{ 0.0f },
        m_setTimeOfDayHook{}
    {
        log("[FreezeTimeOfDay] Entering constructor...");

        g_freezeTimeOfDay = this;

        auto address = scan("client.exe", "55 8B EC 8B 01 D9 45 08 D9 58 0C");

        if (address) {
            log("[FreezeTimeOfDay] Found address for setTimeOfDay %p", *address);

            m_setTimeOfDayHook = make_unique<FunctionHook>(*address, (uintptr_t)&FreezeTimeOfDay::hookedSetTimeOfDay);

            if (m_setTimeOfDayHook->isValid()) {
                log("[FreezeTimeOfDay] Hooked setTimeOfDay");
            }
            else {
                log("[FreezeTimeOfDay] Failed to hook setTimeOfDay");
            }
        }
        else {
            log("[FreezeTimeOfDay] Failed to find address for setTimeOfDay");
        }

        log("[FreezeTimeOfDay] Leaving constructor");
    }

    FreezeTimeOfDay::~FreezeTimeOfDay() {
        m_setTimeOfDayHook->remove();

        g_freezeTimeOfDay = nullptr;
    }

    void FreezeTimeOfDay::onUI() {
        if (m_setTimeOfDayHook == nullptr || !m_setTimeOfDayHook->isValid()) {
            return;
        }

        if (ImGui::CollapsingHeader("Freeze Time Of Day")) {
            ImGui::TextWrapped("Freezes the time of day at whatever value you want.");
            ImGui::Spacing();
            ImGui::Checkbox("Is Enabled", &m_isEnabled);
            ImGui::SliderFloat("Time Of Day", &m_timeOfDay, 0.0f, 24.0f, "%.1f");
        }
    }

    void FreezeTimeOfDay::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("FreezeTimeOfDay.Enabled").value_or(false);
        m_timeOfDay = cfg.get<float>("FreezeTimeOfDay.Time").value_or(0.0f);
    }

    void FreezeTimeOfDay::onConfigSave(Config& cfg) {
        cfg.set<bool>("FreezeTimeOfDay.Enabled", m_isEnabled);
        cfg.set<float>("FreezeTimeOfDay.Time", 0.0f);
    }

    int FreezeTimeOfDay::hookedSetTimeOfDay(uintptr_t object, float timeOfDay) {
        auto orig = (decltype(hookedSetTimeOfDay)*)g_freezeTimeOfDay->m_setTimeOfDayHook->getOriginal();
        
        if (g_freezeTimeOfDay->m_isEnabled) {
            return orig(object, g_freezeTimeOfDay->m_timeOfDay / 24.0f);
        }

        return orig(object, timeOfDay);
    }
}