#pragma once

#include <d3d9.h>

class Renderer {
public:
    Renderer() = default;
    Renderer(const Renderer& other);
    Renderer(Renderer&& other) noexcept;
    Renderer(HWND window);
    virtual ~Renderer();

    void resize(int w, int h);

    auto getWindow() {
        return m_wnd;
    }

    auto getDevice() {
        return m_d3dDevice;
    }

    auto& getPresentParams() {
        return m_d3dpp;
    }

    auto isValid() const {
        return m_d3dDevice != nullptr;
    }

    Renderer& operator=(const Renderer& other);
    Renderer& operator=(Renderer&& other) noexcept;

private:
    HWND m_wnd{};
    IDirect3D9* m_d3d{};
    IDirect3DDevice9* m_d3dDevice{};
    D3DPRESENT_PARAMETERS m_d3dpp{};
};
