#include <imgui.h>
#include <sstream>
#include <iomanip>

#include <Scan.hpp>

#include "Log.hpp"
#include "SecondaryPassword.hpp"
#include "String.hpp"


using namespace std;

namespace kanan {
    static SecondaryPassword* g_secondaryPassword{ nullptr };
	static bool g_secondaryPasswordAutoLogin;
	static int g_secondaryPasswordAutoConfirm_tries { 0 };

    SecondaryPassword::SecondaryPassword()
        : m_isEnabled{ false },
        password{ nullptr },
        savedPassword{ 0 },
        m_setCVisualKeyboardViewOnPostCreateHook{},
        m_CStringAllocate{}
    {
        g_secondaryPassword = this;
    }

    string SecondaryPassword::wstringToHex(wstring input)
    {
        return memoryToHex(&input[0], input.length() * 2);
    }

    string SecondaryPassword::memoryToHex(void *p, int size)
    {
        std::ostringstream result;
        result << std::internal << std::setfill('0') << std::hex << std::uppercase;
        auto buffer = reinterpret_cast<const uint8_t*>(p);
        for (auto i = 0; i < size; ++i)
            result << setw(2) << (int)buffer[i];
        return result.str();
    }

    bool SecondaryPassword::InitCStrings()
    {
        // looking for CString allocator
        auto someToken_addr = scan("client.exe", wstringToHex(L"&#xD;"));
        if (someToken_addr)
        {
            if (0) log("[SecondaryPassword] Found address of &#xD; %p", someToken_addr);
        }
        else {
            log("[SecondaryPassword] Failed to get address of address of &#xD;");
            return false;
        }
        auto someXmlFunc_addr = scan("client.exe", "68" + memoryToHex(&*someToken_addr, 4)); // push string
        if (someXmlFunc_addr)
        {
            someXmlFunc_addr = scan(*someXmlFunc_addr, 20, "E8 ? ? ? FF"); // call
        }
        if (someXmlFunc_addr)
        {
            someXmlFunc_addr = scan(*someXmlFunc_addr + 1, 20, "E8 ? ? ? FF"); // call CString allocator
        }
        if (someXmlFunc_addr)
        {
            auto cStringAllocator_offset = *reinterpret_cast<intptr_t*>(*someXmlFunc_addr + 1);
            if (0) log("[SecondaryPassword] cStringAllocator_offset %d", cStringAllocator_offset);
            m_CStringAllocate = reinterpret_cast<CString *(*)(int)>(reinterpret_cast<uintptr_t*>(*someXmlFunc_addr + 5 + cStringAllocator_offset));
            // todo: add sanity check
            if (0) log("[SecondaryPassword] Found address of CString allocator %p", m_CStringAllocate);
        }
        else {
            log("[SecondaryPassword] Failed to get address of CString allocator");
            return false;
        }
        return true;
    }

    CString* SecondaryPassword::CStringConstructor(wchar_t* str)
    {
        auto len = wcslen(str);
        CString* result = m_CStringAllocate(len + 1);
        if(result)
        {
            wcsncpy_s(result->buffer, str, len + 1);
            result->length = len;
        }
        return result;
    }

    void SecondaryPassword::setup()
    {
        log("[SecondaryPassword] Entering setup...");
        if (!InitCStrings()) return;

        // Looking for pleione::CVisualKeyboardView constructor
        auto PLI_WindowVisualKeyboard_7_addr = scan("client.exe", wstringToHex(L"code.interface.PLI_WindowVisualKeyboard.7"));
        if (PLI_WindowVisualKeyboard_7_addr)
        {
            if (0) log("[SecondaryPassword] Found code.interface.PLI_WindowVisualKeyboard.7: %p", PLI_WindowVisualKeyboard_7_addr);
        }
        else {
            log("[SecondaryPassword] Failed to get address of \"code.interface.PLI_WindowVisualKeyboard.7\"");
            return;
        }
        auto constructorTmpAddr = scan("client.exe", "68" + memoryToHex(&*PLI_WindowVisualKeyboard_7_addr, 4));
        if (constructorTmpAddr)
        {
            if (0) log("[SecondaryPassword] Found pleione::CVisualKeyboardView constructor: %p", constructorTmpAddr);
        }
        else {
            log("[SecondaryPassword] Failed to get address of pleione::CVisualKeyboardView constructor");
            return;
        }
        // Looking for pleione::CVisualKeyboardView vtable
        auto cVisualKeyboardViewVtableTmp = scan(*constructorTmpAddr - 300, 1000, "C7 06 ? ? ? ? 89");
        uintptr_t *cVisualKeyboardViewVtable;
        if (cVisualKeyboardViewVtableTmp)
        {
            cVisualKeyboardViewVtable = reinterpret_cast<uint32_t*>(*reinterpret_cast<uint32_t*>(*cVisualKeyboardViewVtableTmp + 2));
            if (0) log("[SecondaryPassword] Found pleione::CVisualKeyboardView vtable: %p", cVisualKeyboardViewVtable);
        }
        else {
            log("[SecondaryPassword] Failed to get address of pleione::CVisualKeyboardView vtable");
            return;
        }

        // RIP: 'type cast': cannot convert from 'void (__thiscall kanan::CWindow::* )(int *)' to 'void *'
        // log("[SecondaryPassword] pleione::CVisualKeyboardView::OnPostCreate addr: %p", (void*)(tempViewPtr->OnPostCreate));
        if (0) log("[SecondaryPassword] pleione::CVisualKeyboardView::OnPostCreate addr: %p", cVisualKeyboardViewVtable[88]);
        m_setCVisualKeyboardViewOnPostCreateHook = make_unique<FunctionHook>(cVisualKeyboardViewVtable[88], (uintptr_t)&SecondaryPassword::hookedCVisualKeyboardViewOnPostCreate);
        // todo: sanity check
        log("[SecondaryPassword] Leaving setup...");
    }

    void SecondaryPassword::hookedCVisualKeyboardViewOnPostCreate(CVisualKeyboardView* view, uint32_t EDX, int metaData) {
        auto orig = (decltype(hookedCVisualKeyboardViewOnPostCreate)*)g_secondaryPassword->m_setCVisualKeyboardViewOnPostCreateHook->getOriginal();
        orig(view, EDX, metaData);

        if (view->Mode != 7) return; // not Second Password Keyboard
        if (view->isFirstUse) return;
        if (strnlen(g_secondaryPassword->savedPassword, 60) == 0) return;

        wchar_t tempString[60];
        size_t numOfCharConverted;
        mbstowcs_s(&numOfCharConverted, tempString, g_secondaryPassword->savedPassword, 60);
        g_secondaryPassword->password = g_secondaryPassword->CStringConstructor(tempString);
        if (g_secondaryPassword->password)
        {
            view->inputBox->SetTitle(&g_secondaryPassword->password);
            view->okButton->SetEnable(true);
			if(g_secondaryPasswordAutoLogin 
				&& g_secondaryPasswordAutoConfirm_tries == 0)//avoid wrong password message loop
			{ 
				view->okButton->OnMouseLButtonDown(0, 0, true);
				view->okButton->OnMouseLButtonUp(0, 0, true);
				g_secondaryPasswordAutoConfirm_tries++;
			}
        }
        else
        {
            log("[SecondaryPassword] can't allocate CString");
        }
    }

    void SecondaryPassword::onUI() {
        if (ImGui::CollapsingHeader("Second Password")) {
            ImGui::TextWrapped(
                "Auto inputs second password for you. "
                "NOTE: You must restart the game after enabling or disabling this mod for it take effect."
            );
            ImGui::Spacing();
			ImGui::Checkbox("Enable Secondary Password", &m_isEnabled);
			ImGui::Checkbox("Enable Secondary Password Auto Login", &g_secondaryPasswordAutoLogin);
            ImGui::InputText("Password:", savedPassword, 60, ImGuiInputTextFlags_Password);
        }
    }

    void SecondaryPassword::onConfigLoad(const Config& cfg) {
		m_isEnabled = cfg.get<bool>("SecondaryPassword.Enabled").value_or(false);

        auto pass = cfg.get("SecondaryPassword.Password").value_or("");
        auto len = pass.length();
        len = len < 59 ? len : 59;
        pass.copy(savedPassword, len, 0);
        savedPassword[len] = 0;
 
        if (m_isEnabled) setup();

		g_secondaryPasswordAutoLogin = cfg.get<bool>("SecondaryPassword.AutoLogin.Enabled").value_or(false);
    }

    void SecondaryPassword::onConfigSave(Config& cfg) {
        cfg.set<bool>("SecondaryPassword.Enabled", m_isEnabled);
        cfg.set("SecondaryPassword.Password", savedPassword);
		cfg.set<bool>("SecondaryPassword.AutoLogin.Enabled", g_secondaryPasswordAutoLogin);
    }
}
