#pragma once

#include <Windows.h>

#include "Mod.hpp"

namespace kanan {
    class EnableMultiClient : public Mod {
    public:
        EnableMultiClient();

        void onUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        bool m_isEnabled;
        HANDLE** m_handlePtr;

        void apply();
    };
}
