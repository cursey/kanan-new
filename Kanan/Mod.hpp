#pragma once

#include <Config.hpp>

namespace kanan {
    class Mod {
    public:
        virtual bool onInitialize() { return true; };
        virtual void onFrame() {};

        virtual void onUI() {};
        virtual void onPatchUI() {};

        virtual void onConfigLoad(const Config& cfg) {};
        virtual void onConfigSave(Config& cfg) {};
    };
}
