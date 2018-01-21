#pragma once

#include <memory>

#include <FunctionHook.hpp>

#include "Mod.hpp"

namespace kanan {
    class FreezeTimeOfDay : public Mod {
    public:
        FreezeTimeOfDay();
        virtual ~FreezeTimeOfDay();

        void onUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        bool m_isEnabled;
        float m_timeOfDay;
        std::unique_ptr<FunctionHook> m_setTimeOfDayHook;

        static int __fastcall hookedSetTimeOfDay(uintptr_t object, float timeOfDay);
    };
}
