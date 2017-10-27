#pragma once

#include <memory>

#include <FunctionHook.hpp>

#include "Mod.hpp"

namespace kanan {
    class DisableNagle : public Mod {
    public:
        DisableNagle();
        virtual ~DisableNagle();

        void onUI() override;

        void onConfigLoad(ConfigPtr cfg) override;
        void onConfigSave(ConfigPtr cfg) override;

    private:
        bool m_isEnabled;
        std::unique_ptr<FunctionHook> m_hook;

        static UINT_PTR __stdcall socket(int af, int type, int protocol);
    };
}
