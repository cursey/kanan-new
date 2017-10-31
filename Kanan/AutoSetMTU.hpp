#pragma once

#include <array>
#include <string>
#include <optional>
#include <memory>

#include <FunctionHook.hpp>

#include "Mod.hpp"

namespace kanan {
    class AutoSetMTU : public Mod {
    public:
        AutoSetMTU();

        void onUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        bool m_isEnabled;
        std::array<char, 100> m_interface;
        int m_lowMTU;
        int m_normalMTU;
        std::unique_ptr<FunctionHook> m_hook;

        std::optional<DWORD> runProcess(const std::string& name, const std::string& params);

        static char __cdecl createConnection(int a1, int a2);
    };
}
