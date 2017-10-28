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
                log("DInputHook hooked successfully.");
                g_dinputHook = this;
            }
            else {
                log("DInputHook failed to hook.");
            }
        }
    }

    DInputHook::~DInputHook() {
        // Explicitly unhook the methods we hooked so we can reset g_dinputHook.
        m_getDeviceDataHook.reset();

        g_dinputHook = nullptr;
    }

    bool DInputHook::hook() {
        log("Entering DInputHook::hook().");

        // All we do here is create an IDirectInputDevice so that we can get the 
        // addresses of the methods we want to hook from its vtable.
        using DirectInput8CreateFn = HRESULT(WINAPI*)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);

        auto dinput8 = LoadLibrary(widen("dinput8.dll").c_str());
        auto dinput8Create = (DirectInput8CreateFn)GetProcAddress(dinput8, "DirectInput8Create");

        if (dinput8Create == nullptr) {
            log("Failed to find DirectInput8Create.");
            return false;
        }

        log("Got DirectInput8Create %p", dinput8Create);

        auto instance = (HINSTANCE)GetModuleHandle(nullptr);
        IDirectInput* dinput{ nullptr };

        if (FAILED(dinput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8W, (LPVOID*)&dinput, nullptr))) {
            log("Failed to create IDirectInput.");
            return false;
        }
        
        log("Got IDirectInput %p", dinput);

        IDirectInputDevice* device{ nullptr };

        if (FAILED(dinput->CreateDevice(GUID_SysKeyboard, &device, nullptr))) {
            log("Failed to create IDirectInputDevice.");
            dinput->Release();
            return false;
        }

        log("Got IDirectInputDevice %p", device);

        // Get the addresses of the methods we want to hook.
        auto getDeviceData = (*(uintptr_t**)device)[10];

        log("Got IDirectInputDevice::GetDeviceData %p", getDeviceData);

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