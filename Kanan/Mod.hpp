#pragma once

#include <cpptoml.h>

namespace kanan {
    class Mod {
    public:
        using ConfigPtr = std::shared_ptr<cpptoml::table>;

        virtual bool onInitialize() { return true; };
        virtual void onFrame() {};

        virtual void onUI() {};
        virtual void onPatchUI() {};

        virtual void onConfigLoad(ConfigPtr cfg) {};
        virtual void onConfigSave(ConfigPtr cfg) {};
    };
}
