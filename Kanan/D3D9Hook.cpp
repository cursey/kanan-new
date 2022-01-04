#include <String.hpp>

#include "Log.hpp"
#include "D3D9Hook.hpp"

using namespace std;

namespace kanan {
    static D3D9Hook* g_d3d9Hook{ nullptr };

    D3D9Hook::D3D9Hook()
        : onPresent{},
        onPreReset{},
        onPostReset{},
        m_device{ nullptr },
        m_presentHook{ nullptr },
        m_resetHook{ nullptr }
    {
        if (g_d3d9Hook == nullptr) {
            if (hook()) {
                log("D3D9Hook hooked successfully.");
            }
            else {
                log("D3D9Hook failed to hook.");
            }
        }
    }

    D3D9Hook::~D3D9Hook() {
        // Explicitly unhook the methods we hooked so we can reset g_d3d9Hook.
        m_presentHook.reset();
        m_resetHook.reset();

        g_d3d9Hook = nullptr;
    }

    bool D3D9Hook::hook() {
        log("Entering D3D9Hook::hook().");

        // Set hook object preemptively -- otherwise, the hook is written and is likely
        // to execute and crash before we verify success.
        g_d3d9Hook = this;

        // All we do here is create a IDirect3DDevice9 so that we can get the address
        // of the methods we want to hook from its vtable.
        using D3DCreate9Fn = IDirect3D9* (WINAPI*)(UINT);

        auto d3d9 = LoadLibrary(widen("d3d9.dll").c_str());
        auto d3dCreate9 = (D3DCreate9Fn)GetProcAddress(d3d9, "Direct3DCreate9");

        if (d3dCreate9 == nullptr) {
            log("Couldn't find Direct3DCreate9.");
            return false;
        }

        log("Got Direct3DCreate9 %p", d3dCreate9);

        auto d3d = d3dCreate9(D3D_SDK_VERSION);

        if (d3d == nullptr) {
            log("Failed to create IDirect3D9.");
            return false;
        }

        log("Got IDirect3D9 %p", d3d);

        D3DPRESENT_PARAMETERS pp{};

        ZeroMemory(&pp, sizeof(pp));

        pp.Windowed = 1;
        pp.SwapEffect = D3DSWAPEFFECT_FLIP;
        pp.BackBufferFormat = D3DFMT_A8R8G8B8;
        pp.BackBufferCount = 1;
        pp.hDeviceWindow = GetDesktopWindow();
        pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        IDirect3DDevice9* device{ nullptr };

        if (FAILED(d3d->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_NULLREF,
            GetDesktopWindow(),
            D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_NOWINDOWCHANGES,
            &pp,
            &device)))
        {
            log("Failed to create IDirect3DDevice9.");
            d3d->Release();
            return false;
        }

        log("Got IDirect3DDevice9 %p", device);

        // Grab the addresses of the methods we want to hook.
        auto present = (*(uintptr_t**)device)[17];
        auto reset = (*(uintptr_t**)device)[16];

        log("Got IDirect3DDevice9::Present %p", present);
        log("Got IDirect3DDevice9::Reset %p", reset);

        device->Release();
        d3d->Release();

        // Hook them.
        m_presentHook = make_unique<FunctionHook>(present, (uintptr_t)&D3D9Hook::present);
        m_resetHook = make_unique<FunctionHook>(reset, (uintptr_t)&D3D9Hook::reset);

        if (m_presentHook->isValid() && m_resetHook->isValid()) {
            return true;
        }
        else {
            // If a problem occurred, reset the hook.
            m_presentHook.reset();
            m_resetHook.reset();
            g_d3d9Hook = nullptr;
            return false;
        }
    }

    HRESULT D3D9Hook::present(IDirect3DDevice9* device, CONST RECT* src, CONST RECT* dest, HWND wnd, CONST RGNDATA* dirtyRgn) {
        auto d3d9 = g_d3d9Hook;

        d3d9->m_device = device;

        // Call our present callback.
        if (d3d9->onPresent) {
            d3d9->onPresent(*d3d9);
        }

        // Call the original present.
        auto originalPresent = (decltype(D3D9Hook::present)*)d3d9->m_presentHook->getOriginal();

        return originalPresent(device, src, dest, wnd, dirtyRgn);
    }

    HRESULT D3D9Hook::reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentParams) {
        auto d3d9 = g_d3d9Hook;

        d3d9->m_device = device;

        // Call our pre reset callback.
        if (d3d9->onPreReset) {
            d3d9->onPreReset(*d3d9);
        }

        // Call the original reset.
        auto originalReset = (decltype(D3D9Hook::reset)*)d3d9->m_resetHook->getOriginal();
        auto result = originalReset(device, presentParams);

        // Call our post reset callback.
        if (result == D3D_OK && d3d9->onPostReset) {
            d3d9->onPostReset(*d3d9);
        }

        return result;
    }
}
