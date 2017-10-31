#pragma once

#include <functional>
#include <memory>

#include <d3d9.h>

#include <FunctionHook.hpp>

namespace kanan {
    class D3D9Hook {
    public:
        //
        // Callbacks to actually do work when the hooked functions get called.
        //
        std::function<void(D3D9Hook&)> onPresent;
        std::function<void(D3D9Hook&)> onPreReset;
        std::function<void(D3D9Hook&)> onPostReset;

        D3D9Hook();
        D3D9Hook(const D3D9Hook& other) = delete;
        D3D9Hook(D3D9Hook&& other) = delete;
        virtual ~D3D9Hook();

        auto getDevice() const {
            return m_device;
        }

        auto isValid() const {
            return m_presentHook->isValid() && m_resetHook->isValid();
        }

        D3D9Hook& operator=(const D3D9Hook& other) = delete;
        D3D9Hook& operator=(D3D9Hook&& other) = delete;

    private:
        IDirect3DDevice9* m_device;

        std::unique_ptr<FunctionHook> m_presentHook;
        std::unique_ptr<FunctionHook> m_resetHook;

        bool hook();

        static HRESULT WINAPI present(IDirect3DDevice9* device, CONST RECT* src, CONST RECT* dest, HWND wnd, CONST RGNDATA* dirtyRgn);
        static HRESULT WINAPI reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentParams);
    };
}
