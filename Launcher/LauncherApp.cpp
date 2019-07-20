#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <imgui.h>
#include <imgui_freetype.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include <json.hpp>

#include <Config.hpp>
#include <FontData.hpp>
#include <String.hpp>

#include "BackgroundPng.hpp"
#include "Crypto.hpp"
#include "HttpClient.hpp"
#include "WmiQuery.hpp"
#include "RegQuery.hpp"
#include "LauncherApp.hpp"

using namespace std;
using nlohmann::json;
using kanan::narrow;
using kanan::widen;
namespace fs = std::filesystem;

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

string getDeviceID() {
    string uuid{};

    uuid += wmiQueryString("SELECT * FROM Win32_ComputerSystemProduct", "UUID");
    uuid += regQueryString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", "MachineGuid");

    auto uuidHash = hashString("SHA256", uuid);

    return stringEncode(CRYPT_STRING_HEXRAW, uuidHash);
}

LauncherApp::LauncherApp()
    : App{ "Kanan Launcher", 800, 600 },
    m_renderer{ m_window },
    m_bg{ m_renderer.getDevice(), g_backgroundPng, sizeof(g_backgroundPng) }
{
    // Initialize ImGui.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    auto& io = ImGui::GetIO();

    io.IniFilename = "ui.ini";

    io.Fonts->AddFontFromMemoryCompressedTTF(g_font_compressed_data, g_font_compressed_size, 16.0f);
    ImGuiFreeType::BuildFontAtlas(io.Fonts, 0);

    if (!ImGui_ImplWin32_Init(m_window)) {
        throw runtime_error{ "Failed to initialize ImGui Win32 component" };
    }

    if (!ImGui_ImplDX9_Init(m_renderer.getDevice())) {
        throw runtime_error{ "Failed to initialize ImGui Direct3D component" };
    }

    ImGui::StyleColorsDark();

    if (!fs::exists("profiles.dat")) {
        m_openFirstTimeSetup = true;
    }
    else {
        m_openUnlockPrompt = true;
    }
}

LauncherApp::~LauncherApp() {
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

LRESULT LauncherApp::onMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(wnd, msg, wParam, lParam)) {
        return 0;
    }

    switch (msg) {
    case WM_SIZE:
        if (m_renderer.isValid() && wParam != SIZE_MINIMIZED) {
            ImGui_ImplDX9_InvalidateDeviceObjects();

            m_windowW = LOWORD(lParam);
            m_windowH = HIWORD(lParam);

            m_renderer.resize(m_windowW, m_windowH);
            ImGui_ImplDX9_CreateDeviceObjects();
        }
        return 0;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) { // Disable ALT application menu
            return 0;
        }

        break;

    case WM_QUIT:
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    default:
        break;
    }

    return App::onMessage(wnd, msg, wParam, lParam);
}

void LauncherApp::onFrame() {
    drawUI();

    auto device = m_renderer.getDevice();

    // Make sure we are good to begin rendering.
    auto coopLevel = device->TestCooperativeLevel();

    if (coopLevel != D3D_OK) {
        // If the device isn't reset, reset it.
        if (coopLevel == D3DERR_DEVICENOTRESET) {
            ImGui_ImplDX9_InvalidateDeviceObjects();

            if (device->Reset(&m_renderer.getPresentParams()) == D3D_OK) {
                ImGui_ImplDX9_CreateDeviceObjects();
            }
        }

        return;
    }

    device->SetRenderState(D3DRS_ZENABLE, false);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
    device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

    if (device->BeginScene() >= 0) {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        device->EndScene();
    }

    device->Present(NULL, NULL, NULL, NULL);
}

bool LauncherApp::loadProfiles() {
    auto f = ifstream{ "profiles.dat", ifstream::in | ifstream::binary | ifstream::ate };

    if (!f) {
        throw runtime_error{ "Failed to open profiles.dat for reading" };
    }

    vector<uint8_t> ciphertext{};

    ciphertext.resize(f.tellg());
    f.seekg(0, ifstream::beg);
    f.read((char*)ciphertext.data(), ciphertext.size());

    try {
        auto plaintext = decryptData(m_masterPassword.data(), ciphertext);
        auto saveData = json::parse(plaintext);

        if (saveData.at("version").get<int>() != PROFILES_VERSION) {
            // TODO: when version number changes, add migration support or tell
            // the user they'll be starting over from scratch.
            MessageBox(
                m_window,
                L"Your profiles.dat file is using a different version than expected.",
                L"Error",
                MB_ICONINFORMATION | MB_OK
            );
        }

        saveData.at("clientPath").get_to(m_clientPath);

        for (auto profile : saveData.at("profiles")) {
            Profile p{};
            string username{};
            string password{};
            string cmdLine{};

            profile.at("username").get_to(username);
            profile.at("password").get_to(password);

            if (profile.find("cmdLine") != profile.end()) {
                profile.at("cmdLine").get_to(cmdLine);
            }

            strcpy_s(p.username.data(), p.username.size(), username.c_str());
            strcpy_s(p.password.data(), p.password.size(), password.c_str());
            strcpy_s(p.cmdLine.data(), p.cmdLine.size(), cmdLine.c_str());

            m_profiles.emplace_back(p);
        }

        // Add an empty one at the end.
        m_profiles.emplace_back(Profile{});
    }
    catch (const exception& e) {
        MessageBox(m_window, widen(e.what()).c_str(), L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    return true;
}

void LauncherApp::saveProfiles() {
    // Only save when the master passwords match!
    if (string_view{ m_masterPassword.data() } != string_view{ m_masterPasswordRepeat.data() }) {
        return;
    }

    auto profiles = json{};

    for (auto& profile : m_profiles) {
        // Skip any empty profiles.
        if (string_view{ profile.username.data() }.empty()) {
            continue;
        }

        profiles.emplace_back(json{
            { "username", string{ profile.username.data() } },
            { "password", string{ profile.password.data() } },
            { "cmdLine", string{ profile.cmdLine.data() } }
         });
    }

    auto saveData = json{
        { "version", PROFILES_VERSION },
        { "clientPath", m_clientPath },
        { "profiles", profiles }
    };
    auto plaintext = saveData.dump();
    auto ciphertext = encryptString(m_masterPassword.data(), plaintext);
    auto f = ofstream{ "profiles.dat", ofstream::out | ofstream::binary };

    if (!f) {
        throw runtime_error{ "Failed to open profiles.dat for writing" };
    }

    f.write((const char*)ciphertext.data(), ciphertext.size());
}

void LauncherApp::drawUI() {
    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //
    // Background window.
    //
    {
        ImGui::SetNextWindowPos(ImVec2{ 0.0f, 0.0f });
        ImGui::SetNextWindowSize(ImVec2{ (float)m_windowW, (float)m_windowH });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{ 0.0f, 0.0f });

        ImGuiWindowFlags bgFlags = ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoInputs;
        auto isBgOpen = ImGui::Begin("##BG", nullptr, bgFlags);

        ImGui::PopStyleVar(3);

        if (isBgOpen) {
            ImVec2 topLeft{ 0.0f, 0.0f };
            ImVec2 bottomRight{ (float)m_windowW / (float)m_bg.getW(), (float)m_windowH / (float)m_bg.getH() };

            // Uncomment this if you don't want tiling background images.
            //if (bottomRight.x > 1.0f) { bottomRight.x = 1.0f; }
            //if (bottomRight.y > 1.0f) { bottomRight.y = 1.0f; }

            ImGui::Image(m_bg, ImVec2{ (float)m_windowW, (float)m_windowH }, topLeft, bottomRight);
        }

        ImGui::End();
    }

    if (m_openFirstTimeSetup || m_isFirstTimeSetupOpen) {
        firstTimeSetupUI();
    }
    else if (m_openUnlockPrompt || m_isUnlockPromptOpen) {
        unlockPromptUI();
    }
    else {
        mainUI();
    }

    // Rendering
    ImGui::EndFrame();
}

void LauncherApp::mainUI() {
    auto openAbout = false;
    auto openLaunchingPrompt = false;
    auto openCustomizeCommandLine = false;

    //
    // Menu bar.
    //
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save")) { saveProfiles(); }
            if (ImGui::MenuItem("Exit")) { PostQuitMessage(0); }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::MenuItem("Options")) { m_isOptionsOpen = true; }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) { openAbout = true; }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    //
    // Main window.
    //
    ImGui::SetNextWindowSize(ImVec2{ 450.0f, -1.0f }, ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2{ 70.0f, 70.0f }, ImGuiCond_Once);

    if (ImGui::Begin("Launcher")) {
        string profileChoices{};

        for (auto& profile : m_profiles) {
            if (string_view{ profile.username.data() }.empty()) {
                profileChoices += string{ "New profile" } + '\0';
            }
            else {
                profileChoices += string{ profile.username.data() } +'\0';
            }
        }

        profileChoices += '\0';

        ImGui::Combo("Profile", &m_currentProfile, profileChoices.c_str());

        auto& profile = m_profiles[m_currentProfile];
        auto& username = profile.username;
        auto& password = profile.password;

        ImGui::InputText("Username/E-mail", username.data(), username.size());
        ImGui::InputText("Password", password.data(), password.size(), ImGuiInputTextFlags_Password);

        if (ImGui::Button("Customize Command Line")) {
            openCustomizeCommandLine = true;
        }

        if (ImGui::Button("Launch Client")) {
            saveProfiles();
            m_launchResult = async(launch::async, [=] {
                try {
                    // Get the API access token.
                    auto sha512_password = stringEncode(CRYPT_STRING_HEXRAW, hashString("SHA512", password.data()));
                    auto device_id = getDeviceID();
                    string header{ "Content-Type: application/json" };
                    auto body = json{
                        { "id", string_view{ username.data() } },
                        { "password", sha512_password },
                        { "client_id", "7853644408" },
                        { "scope", "us.launcher.all" },
                        { "device_id", device_id },
                        { "captcha_token", "0xDEADBEEF" }
                    }.dump();
                    auto response = httpPost("https://www.nexon.com/account-webapi/login/launcher", header, body);
                    auto json_response = json::parse(response);
                    auto id_token = json_response.at("id_token").get<string>();
                    auto access_token = json_response.at("access_token").get<string>();

                    // Get the product ID.
                    body = json{
                        { "product_id", "10200" },
                        { "id_token", id_token },
                        { "device_id", device_id }
                    }.dump();
                    response = httpPost("https://api.nexon.io/game-auth/v2/check-playable", header, body);
                    json_response = json::parse(response);

                    auto product_id = json_response.at("product_id").get<string>();

                    // Get the passport.
                    header = (ostringstream{} <<
                        "Content-Type: application/json\r\n" <<
                        "Authorization: Bearer " << access_token).str();
                    body = json{
                        { "product_id", product_id }
                    }.dump();
                    response = httpPost("https://api.nexon.io/passport/v1/passport", header, body);
                    json_response = json::parse(response);

                    // Startup the client.
                    STARTUPINFO si{};
                    PROCESS_INFORMATION pi{};
                    auto cmdLine = string{ profile.cmdLine.data() };
                    
                    if (cmdLine.empty()) {
                        cmdLine = (ostringstream{} <<
                            "\"" << m_clientPath << "\" " <<
                            "code:1622 " <<
                            "verstr:248 " <<
                            "ver:248 " <<
                            "locale:USA " <<
                            "env:Regular " <<
                            "setting:file://data/features.xml " <<
                            "logip:35.162.171.43 " <<
                            "logport:11000 " <<
                            "chatip:54.214.176.167 " <<
                            "chatport:8002 " <<
                            "/P:" << json_response.at("passport").get<string>() << " " <<
                            "-bgloader").str();
                    }
                    else {
                        // The user is allowed to use newlines when specifying their
                        // custom command line. We need to strip newlines when we 
                        // actually luanch the program.
                        replace(cmdLine.begin(), cmdLine.end(), '\n', ' ');

                        // Passport also needs to be added.
                        cmdLine += " /P:" + json_response.at("passport").get<string>();
                    }

                    auto workDir = fs::path{ m_clientPath }.remove_filename().native();

                    si.cb = sizeof(si);

                    if (CreateProcess(
                        nullptr,
                        (LPWSTR)widen(cmdLine).c_str(),
                        nullptr,
                        nullptr,
                        FALSE,
                        0,
                        nullptr,
                        workDir.c_str(),
                        &si,
                        &pi
                    ) == FALSE) {
                        throw runtime_error{ "Failed to create the process" };
                    }

                    CloseHandle(pi.hThread);
                    CloseHandle(pi.hProcess);
                }
                catch (const exception& e) {
                    MessageBox(m_window, widen(e.what()).c_str(), L"Error", MB_ICONERROR | MB_OK);
                    return false;
                }

                return true;
            });
            openLaunchingPrompt = true;
        }

        ImGui::SameLine();

        if (ImGui::Button("New")) {
            m_profiles.emplace_back(Profile{});
            m_currentProfile = m_profiles.size() - 1;
        }

        if (m_profiles.size() > 1) {
            ImGui::SameLine();

            if (ImGui::Button("Delete")) {
                m_profiles.erase(m_profiles.begin() + m_currentProfile);
                m_currentProfile = 0;
            }
        }
    }

    ImGui::End();

    //
    // Launching prompt.
    //
    if (openLaunchingPrompt) {
        ImGui::OpenPopup("Launching...");
        openLaunchingPrompt = false;
    }

    ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2{ 450.0f, -1.0f });

    if (ImGui::BeginPopupModal("Launching...")) {
        ImGui::TextWrapped("Attempting to launch Client.exe with the selected profile...");

        if (m_launchResult.wait_for(chrono::seconds(0)) == future_status::ready) {
            ImGui::CloseCurrentPopup();

            if (!m_launchResult.get()) {
                MessageBox(
                    m_window,
                    L"Failed to start Client.exe! Please check your settings and make "
                    L"sure you can login to the account from this computer normally.",
                    L"Error",
                    MB_ICONEXCLAMATION | MB_OK
                );
            }
        }

        ImGui::EndPopup();
    }

    //
    // Customize command line.
    //
    if (openCustomizeCommandLine) {
        ImGui::OpenPopup("Customize Command Line");
        openCustomizeCommandLine = false;
    }

    //ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2{ 450.0f, -1.0f }, ImGuiCond_Once);

    if (ImGui::BeginPopupModal("Customize Command Line")) {
        ImGui::TextWrapped(
            "Here you can completely control the command line used by Kanan Launcher "
            "to launch Mabinogi. You will need to include the executable you wish to "
            " launch as the first peice of the command line. Leave this empty if you "
            "want to use the default command line. You can also use multiple lines "
            "to organize the options if you want (they are stripped when actually "
            "launching). Your login passport will also automatically be appended. "
        );

        auto& profile = m_profiles[m_currentProfile];

        ImGui::InputTextMultiline("##Command Line", profile.cmdLine.data(), profile.cmdLine.size());// , ImVec2{ 400, 200 });

        if (ImGui::Button("Finish")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    //
    // Options window.
    //
    if (m_isOptionsOpen) {
        optionsUI();
    }

    //
    // About popup.
    //
    if (openAbout) {
        ImGui::OpenPopup("About");
        m_isAboutOpen = true;
        openAbout = false;
    }

    if (m_isAboutOpen) {
        ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2{ 450.0f, -1.0f });

        if (ImGui::BeginPopupModal("About", &m_isAboutOpen)) {
            ImGui::TextWrapped(
                "Kanan Launcher is a simple utility to help you launch multiple Mabinogi "
                "clients. Just set the client path then use your normal Nexon login "
                "credentials to start Mabinogi. If you want to launch more than one "
                "client, make sure you have a Multi-Client patch enabled (one is included "
                "with Kanan, make sure its turned on). Finally, make sure to run Kanan "
                "Launcher as an administrator."
            );
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 0.1f, 0.1f, 1.0f });
            ImGui::TextWrapped(
                "NOTE: Kanan is an open-source project. If you have downloaded Kanan "
                "from a trusted source then using Kanan Launcher is safe and your login "
                "information is only ever sent to Nexon servers for login purposes. "
                "If you downloaded Kanan from an untrusted source then PLEASE do not "
                "use Kanan Launcher."
            );
            ImGui::PopStyleColor();
            ImGui::EndPopup();
        }
    }
}

void LauncherApp::optionsUI() {
    ImGui::SetNextWindowSize(ImVec2{ 450.0f, -1.0f }, ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Options", &m_isOptionsOpen)) {
        ImGui::TextWrapped("Make sure the Client Path is set to Mabinogi's Client.exe");

        if (ImGui::Button("...")) {
            OPENFILENAME ofn{};
            wstring filePath{};

            filePath.resize(1024);

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = m_window;
            ofn.hInstance = GetModuleHandle(nullptr);
            ofn.lpstrFilter = L"Client.exe\0*.exe\0\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrTitle = L"Client.exe";
            ofn.lpstrFile = filePath.data();
            ofn.nMaxFile = filePath.size();
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;


            if (GetOpenFileName(&ofn) != FALSE) {
                m_clientPath = narrow(filePath.substr(0, filePath.find_last_not_of(L'\0') + 1));
            }
        }

        ImGui::SameLine();
        ImGui::InputText("Client Path", m_clientPath.data(), m_clientPath.size(), ImGuiInputTextFlags_ReadOnly);


        ImGui::Spacing();
        ImGui::TextWrapped("Master Password");
        ImGui::InputText("Password", m_masterPassword.data(), m_masterPassword.size(), ImGuiInputTextFlags_Password);
        ImGui::InputText("Repeated", m_masterPasswordRepeat.data(), m_masterPasswordRepeat.size(), ImGuiInputTextFlags_Password);

        auto passwordsMatch = string_view{ m_masterPassword.data() } == string_view{ m_masterPasswordRepeat.data() };

        if (!passwordsMatch) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 0.1f, 0.1f, 1.0f });
            ImGui::TextWrapped("Please make sure you enter your master password correctly TWICE");
            ImGui::PopStyleColor();
        }

        ImGui::Spacing();

        if (ImGui::Button("Save")) {
            if (passwordsMatch) {
                saveProfiles();
                m_isOptionsOpen = false;
            }
            else {
                MessageBox(m_window, L"The passwords do not match", L"Error", MB_ICONEXCLAMATION | MB_OK);
            }
        }
    }

    ImGui::End();
}

void LauncherApp::firstTimeSetupUI() {
    auto openSetMasterPassword = false;
    auto openSetClientPath = false;

    if (m_openFirstTimeSetup) {
        ImGui::OpenPopup("First Time Setup");
        m_isFirstTimeSetupOpen = true;
        m_openFirstTimeSetup = false;
    }

    ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2{ 350.0f, -1.0f });

    if (ImGui::BeginPopupModal("First Time Setup")) {
        ImGui::TextWrapped("Welcome to Kanan Launcher!");
        ImGui::Spacing();
        ImGui::TextWrapped(
            "This appears to be the first time you've launched Kanan Launcher so we "
            "will need to set some things up."
        );
        ImGui::Spacing();

        if (ImGui::Button("Lets Begin")) {
            ImGui::CloseCurrentPopup();
            openSetMasterPassword = true;
        }

        ImGui::EndPopup();
    }

    //
    // Master password.
    //
    if (openSetMasterPassword) {
        ImGui::OpenPopup("Set Master Password");
        openSetMasterPassword = false;
    }

    ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2{ 500.0f, -1.0f });

    if (ImGui::BeginPopupModal("Set Master Password")) {
        ImGui::TextWrapped(
            "Please choose a secure master password. This password will be used "
            "to protect your saved login information. You will need to re-enter "
            "it everytime you launch Kanan Launcher to decrypt your saved profiles. "
        );
        ImGui::Spacing();
        ImGui::InputText("Password", m_masterPassword.data(), m_masterPassword.size(), ImGuiInputTextFlags_Password);
        ImGui::InputText("Repeated", m_masterPasswordRepeat.data(), m_masterPasswordRepeat.size(), ImGuiInputTextFlags_Password);

        auto passwordsMatch = string_view{ m_masterPassword.data() } == string_view{ m_masterPasswordRepeat.data() };

        if (ImGui::Button("Set Master Password")) {
            if (passwordsMatch) {
                ImGui::CloseCurrentPopup();
                openSetClientPath = true;
            }
            else {
                MessageBox(m_window, L"The passwords do not match!", L"Error", MB_ICONEXCLAMATION | MB_OK);
            }
        }

        ImGui::Spacing();

        if (!passwordsMatch) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 0.1f, 0.1f, 1.0f });
            ImGui::TextWrapped("Please make sure you enter your master password correctly TWICE");
            ImGui::PopStyleColor();
        }

        ImGui::EndPopup();
    }

    //
    // Client path.
    //
    if (openSetClientPath) {
        ImGui::OpenPopup("Set Client Path");
    }

    ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2{ 500.0f, -1.0f });

    if (ImGui::BeginPopupModal("Set Client Path")) {
        ImGui::TextWrapped("Make sure the Client Path is set to Mabinogi's Client.exe");

        if (ImGui::Button("...")) {
            OPENFILENAME ofn{};
            wstring filePath{};

            filePath.resize(1024);

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = m_window;
            ofn.hInstance = GetModuleHandle(nullptr);
            ofn.lpstrFilter = L"Client.exe\0*.exe\0\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrTitle = L"Client.exe";
            ofn.lpstrFile = filePath.data();
            ofn.nMaxFile = filePath.size();
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;


            if (GetOpenFileName(&ofn) != FALSE) {
                m_clientPath = narrow(filePath.substr(0, filePath.find_last_not_of(L'\0') + 1));
            }
        }

        ImGui::SameLine();
        ImGui::InputText("Client Path", m_clientPath.data(), m_clientPath.size(), ImGuiInputTextFlags_ReadOnly);

        if (ImGui::Button("Save")) {
            saveProfiles();
            ImGui::CloseCurrentPopup();
            m_isFirstTimeSetupOpen = false;

            // At this point there won't be any profiles so insert an empty one.
            m_profiles.emplace_back(Profile{});
        }

        ImGui::EndPopup();
    }
}

void LauncherApp::unlockPromptUI() {
    if (m_openUnlockPrompt) {
        ImGui::OpenPopup("Unlock profiles.dat");
        m_isUnlockPromptOpen = true;
        m_openUnlockPrompt = false;
    }

    ImGui::SetNextWindowPosCenter(ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2{ 500.0f, -1.0f });

    if (ImGui::BeginPopupModal("Unlock profiles.dat")) {
        ImGui::TextWrapped(
            "Please enter your master password to unlock your saved profiles."
        );
        ImGui::InputText("Master Password", m_masterPassword.data(), m_masterPassword.size(), ImGuiInputTextFlags_Password);

        if (ImGui::Button("Unlock")) {
            copy(m_masterPassword.begin(), m_masterPassword.end(), m_masterPasswordRepeat.begin());

            if (loadProfiles()) {
                ImGui::CloseCurrentPopup();
                m_isUnlockPromptOpen = false;
            }
        }

        ImGui::EndPopup();
    }
}

