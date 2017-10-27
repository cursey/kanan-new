#define INITGUID

#include <String.hpp>

#include "Log.hpp"
#include "DInputHook.hpp"

using namespace std;

namespace kanan {
    static DInputHook* g_dinputHook{ nullptr };

    DInputHook::DInputHook(HWND wnd)
        : m_wnd{ wnd },
        m_getDeviceDataHook{ nullptr },
        m_deviceObjectData{},
        m_isIgnoringInput{ false }
    {
        if (g_dinputHook == nullptr) {
            if (hook()) {
                g_log << "DInputHook hooked successfully." << endl;
                g_dinputHook = this;
            }
            else {
                g_log << "DInputHook failed to hook." << endl;
            }
        }
    }

    DInputHook::~DInputHook() {
        // Explicitly unhook the methods we hooked so we can reset g_dinputHook.
        m_getDeviceDataHook.reset();

        g_dinputHook = nullptr;
    }

    bool DInputHook::hook() {
        g_log << "Entering DInputHook::hook()." << endl;

        // All we do here is create an IDirectInputDevice so that we can get the 
        // addresses of the methods we want to hook from its vtable.
        using DirectInput8CreateFn = HRESULT(WINAPI*)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);

        auto dinput8 = LoadLibrary(widen("dinput8.dll").c_str());
        auto dinput8Create = (DirectInput8CreateFn)GetProcAddress(dinput8, "DirectInput8Create");

        if (dinput8Create == nullptr) {
            g_log << "Failed to find DirectInput8Create." << endl;
            return false;
        }

        g_log << "Got DirectInput8Create " << hex << dinput8Create << endl;

        auto instance = (HINSTANCE)GetModuleHandle(nullptr);
        IDirectInput* dinput{ nullptr };

        if (FAILED(dinput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8W, (LPVOID*)&dinput, nullptr))) {
            g_log << "Failed to create IDirectInput." << endl;
            return false;
        }
        
        g_log << "Got IDirectInput " << hex << dinput << endl;

        IDirectInputDevice* device{ nullptr };

        if (FAILED(dinput->CreateDevice(GUID_SysKeyboard, &device, nullptr))) {
            g_log << "Failed to create IDirectInputDevice." << endl;
            dinput->Release();
            return false;
        }

        g_log << "Got IDirectInputDevice " << hex << device << endl;

        // Get the addresses of the methods we want to hook.
        auto getDeviceData = (*(uintptr_t**)device)[10];

        g_log << "Got IDirectInputDevice::GetDeviceData " << hex << getDeviceData << endl;

        device->Release();
        dinput->Release();

        // Hook them.
        m_getDeviceDataHook = make_unique<FunctionHook>(getDeviceData, (uintptr_t)&DInputHook::getDeviceData);

        return m_getDeviceDataHook->isValid();
    }

    HRESULT DInputHook::getDeviceData(IDirectInputDevice* device, DWORD size, LPDIDEVICEOBJECTDATA data, LPDWORD numElements, DWORD flags) {
        auto dinput = g_dinputHook;
        auto originalGetDeviceData = (decltype(DInputHook::getDeviceData)*)dinput->m_getDeviceDataHook->getOriginal();

        // If we are ignoring input then we call the original to remove buffered 
        // input events from the devices queue without modifying the out parameters.
        if (dinput->m_isIgnoringInput) {
            device->Unacquire();
            device->SetCooperativeLevel(dinput->m_wnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
            device->Acquire();

            dinput->m_deviceObjectData.resize(*numElements);

            originalGetDeviceData(device, size, dinput->m_deviceObjectData.data(), numElements, flags);

            *numElements = 0;

            return DI_OK;
        }

        return originalGetDeviceData(device, size, data, numElements, flags);
    }
}