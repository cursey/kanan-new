#pragma once

#include <array>
#include <string>
#include <vector>
#include <future>

#include "App.hpp"
#include "Renderer.hpp"
#include "Image.hpp"

class LauncherApp : public App {
public:
    LauncherApp();
    virtual ~LauncherApp();

    LRESULT onMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
    void onFrame() override;

private:
    static constexpr auto PROFILES_VERSION = 1;

    Renderer m_renderer;
    bool m_isOptionsOpen{};
    bool m_isAboutOpen{};

    struct Profile {
        std::array<char, 64> username;
        std::array<char, 64> password;
        std::array<char, 1024> cmdLine;
    };

    std::vector<Profile> m_profiles{};
    int m_currentProfile = 0;

    std::string m_clientPath{};
    Image m_bg;

    std::array<char, 64> m_masterPassword;
    std::array<char, 64> m_masterPasswordRepeat;

    std::future<bool> m_launchResult{};

    bool m_isFirstTimeSetupOpen{};
    bool m_openFirstTimeSetup{};
    bool m_isUnlockPromptOpen{};
    bool m_openUnlockPrompt{};

    bool loadProfiles();
    void saveProfiles();

    void drawUI();
    void mainUI();
    void optionsUI();
    void firstTimeSetupUI();
    void unlockPromptUI();
};

