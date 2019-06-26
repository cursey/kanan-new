#include <stdexcept>

#include "Renderer.hpp"

using std::runtime_error;

Renderer::Renderer(const Renderer& other)
    : m_wnd{ other.m_wnd },
    m_d3d{ other.m_d3d },
    m_d3dDevice{ other.m_d3dDevice },
    m_d3dpp{ other.m_d3dpp }
{
    if (m_d3d != nullptr) {
        m_d3d->AddRef();
    }

    if (m_d3dDevice != nullptr) {
        m_d3dDevice->AddRef();
    }
}

Renderer::Renderer(Renderer&& other) noexcept
    : m_wnd{ other.m_wnd },
    m_d3d{ other.m_d3d },
    m_d3dDevice{ other.m_d3dDevice },
    m_d3dpp{ other.m_d3dpp }
{
    other.m_d3d = nullptr;
    other.m_d3dDevice = nullptr;
}

Renderer::Renderer(HWND window)
    : m_wnd{ window },
    m_d3d{},
    m_d3dDevice{},
    m_d3dpp{}
{
    m_d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (m_d3d == nullptr) {
        throw runtime_error{ "Failed to create Direct3D object." };
    }

    D3DDISPLAYMODE d3ddm{};

    if (FAILED(m_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
        throw runtime_error{ "Failed to get adapter display mode." };
    }

    // Do we support hardware vertex processing? if so, use it. 
    // If not, downgrade to software.
    D3DCAPS9 d3dCaps{};

    if (FAILED(m_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps))) {
        throw runtime_error{ "Failed to get device capabilities" };
    }

    DWORD behaviorFlags = 0;

    if (d3dCaps.VertexProcessingCaps != 0) {
        behaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
    }
    else {
        behaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    // Everything checks out - create a simple, windowed device.
    m_d3dpp.BackBufferFormat = d3ddm.Format;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.Windowed = TRUE;
    m_d3dpp.EnableAutoDepthStencil = TRUE;
    m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_wnd, behaviorFlags, &m_d3dpp, &m_d3dDevice))) {
        throw runtime_error{ "Failed to create Direct3D device" };
    }
}

Renderer::~Renderer() {
    if (m_d3dDevice != nullptr) {
        m_d3dDevice->Release();
        m_d3dDevice = nullptr;
    }

    if (m_d3d != nullptr) {
        m_d3d->Release();
        m_d3d = nullptr;
    }
}

void Renderer::resize(int w, int h) {
    if (!isValid()) {
        return;
    }

    m_d3dpp.BackBufferWidth = w;
    m_d3dpp.BackBufferHeight = h;

    if (FAILED(m_d3dDevice->Reset(&m_d3dpp))) {
        throw runtime_error{ "Failed to reset Direct3D device during resize" };
    }
}

Renderer & Renderer::operator=(const Renderer& other) {
    m_wnd = other.m_wnd;
    m_d3d = other.m_d3d;
    m_d3dDevice = other.m_d3dDevice;
    m_d3dpp = other.m_d3dpp;

    if (m_d3d != nullptr) {
        m_d3d->AddRef();
    }

    if (m_d3dDevice != nullptr) {
        m_d3dDevice->AddRef();
    }

    return *this;
}

Renderer& Renderer::operator=(Renderer&& other) noexcept {
    m_wnd = other.m_wnd;
    m_d3d = other.m_d3d;
    m_d3dDevice = other.m_d3dDevice;
    m_d3dpp = other.m_d3dpp;
    other.m_d3d = nullptr;
    other.m_d3dDevice = nullptr;

    return *this;
}


