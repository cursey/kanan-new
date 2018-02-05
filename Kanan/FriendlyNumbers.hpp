#pragma once

#include <memory>

#include <FunctionHook.hpp>

#include "Mabinogi.hpp"
#include "Mod.hpp"

namespace kanan {
    class FriendlyNumbers : public Mod {
    public:
        FriendlyNumbers();
        virtual ~FriendlyNumbers();

        void onUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        bool m_isEnabled;
        std::unique_ptr<FunctionHook> m_intToStrHook;

        static CString** hookedIntToStr(CString** a1, unsigned int a2, int a3);
    };
}
