#include <imgui.h>

#include <Scan.hpp>
#include <String.hpp>

#include "Log.hpp"
#include "FriendlyNumbers.hpp"

using namespace std;

namespace kanan {
    static FriendlyNumbers* g_friendlyNumbers{ nullptr };

    FriendlyNumbers::FriendlyNumbers()
        : m_isEnabled{ false },
        m_intToStrHook{}
    {
        log("[FriendlyNumbers] Entering constructor...");

        g_friendlyNumbers = this;

        auto address = scan("client.exe", "55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 83 EC 28 A1 ? ? ? ? 33 C5 89 45 F0 53 56 57 50 8D 45 F4 64 A3 ? ? ? ? 33 C0");

        if (address) {
            log("[FriendlyNumbers] Found address for intToStr %p", *address);

            m_intToStrHook = make_unique<FunctionHook>(*address, (uintptr_t)&FriendlyNumbers::hookedIntToStr);

            if (m_intToStrHook->isValid()) {
                log("[FriendlyNumbers] Hooked intToStr");
            }
            else {
                log("[FriendlyNumbers] Failed to hook intToStr");
            }
        }
        else {
            log("[FriendlyNumbers] Failed to find address for intToStr");
        }

        log("[FriendlyNumbers] Leaving constructor");
    }

    FriendlyNumbers::~FriendlyNumbers() {
        m_intToStrHook->remove();

        g_friendlyNumbers = nullptr;
    }

    void FriendlyNumbers::onUI() {
        if (m_intToStrHook == nullptr || !m_intToStrHook->isValid()) {
            return;
        }

        if (ImGui::CollapsingHeader("Friendly Numbers")) {
            ImGui::TextWrapped("Add's commas to numbers (eg. 10000 -> 10,000)");
            ImGui::Spacing();
            ImGui::Checkbox("Is Enabled", &m_isEnabled);
        }
    }

    void FriendlyNumbers::onConfigLoad(const Config& cfg) {
        m_isEnabled = cfg.get<bool>("FriendlyNumbers.Enabled").value_or(false);
    }

    void FriendlyNumbers::onConfigSave(Config& cfg) {
        cfg.set<bool>("FriendlyNumbers.Enabled", m_isEnabled);
    }

    CString** FriendlyNumbers::hookedIntToStr(CString** a1, unsigned int a2, int a3) {
        auto orig = (decltype(hookedIntToStr)*)g_friendlyNumbers->m_intToStrHook->getOriginal();

        if (g_friendlyNumbers->m_isEnabled && (a3 == 2 || a3 == 7 || a3 == 8)) {
            auto result = orig(a1, a2, a3);
            auto str = *result;
            auto numWithCommas = to_string(a2);
            int insertPosition = (int)numWithCommas.length() - 3;

            while (insertPosition > 0) {
                numWithCommas.insert(insertPosition, ",");
                insertPosition -= 3;
            }

            wcscpy_s(str->buffer, str->capacity, widen(numWithCommas).c_str());
            str->length = numWithCommas.length();

            return result;
        }

        return orig(a1, a2, a3);
    }
}