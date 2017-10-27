#pragma once

#include <memory>
#include <vector>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <FunctionHook.hpp>

namespace kanan {
    class DInputHook {
    public:
        DInputHook() = delete;
        DInputHook(const DInputHook& other) = delete;
        DInputHook(DInputHook&& other) = delete;
        DInputHook(HWND wnd);
        virtual ~DInputHook();

        void ignoreInput() {
            m_isIgnoringInput = true;
        }

        void acknowledgeInput() {
            m_isIgnoringInput = false;
        }

        auto isIgnoringInput() const {
            return m_isIgnoringInput;
        }

        auto isValid() const {
            return m_getDeviceDataHook->isValid();
        }

        DInputHook& operator=(const DInputHook& other) = delete;
        DInputHook& operator=(DInputHook&& other) = delete;

    private:
        HWND m_wnd;

        std::unique_ptr<FunctionHook> m_getDeviceDataHook;

        std::vector<DIDEVICEOBJECTDATA> m_deviceObjectData;

        bool m_isIgnoringInput;

        bool hook();

        static HRESULT WINAPI getDeviceData(IDirectInputDevice* device, DWORD size, LPDIDEVICEOBJECTDATA data, LPDWORD numElements, DWORD flags);
    };
}