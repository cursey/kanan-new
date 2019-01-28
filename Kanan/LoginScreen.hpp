#pragma once

#include <cstdint>

#include "Mod.hpp"

namespace kanan {

    class LoginSceneElement
    {
    public:
        std::string m_name;
        uintptr_t m_constructorAddr;
        LoginSceneElement(std::string name, uintptr_t constructorAddr) : m_name{ name }, m_constructorAddr{ constructorAddr } {};
    };

    class LoginScreen : public Mod {
    private:

    public:
        LoginScreen();

        void onUI() override;

        void onConfigLoad(const Config& cfg) override;
        void onConfigSave(Config& cfg) override;

    private:
        bool m_isEnabled;
        bool m_isApplied;
        bool m_isDebug;
        int m_choice;
        std::string m_sceneName;
        std::string m_comboStr;
        std::vector<LoginSceneElement> m_loginScenes;

        void apply();
    };
}

