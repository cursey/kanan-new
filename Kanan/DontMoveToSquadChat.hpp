#pragma once

#include <Patch.hpp>

#include "PatchMod.hpp"

namespace kanan {
    class DontMoveToSquadChat : public PatchMod {
    public:
        DontMoveToSquadChat();

        void onPatchUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        Patch m_patch;
        bool m_isEnabled;
        unsigned char m_originalByte;
        std::vector<uint8_t> code;
        LPVOID code_address;

        void apply();
    };
}