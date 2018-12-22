#pragma once

#include <cstdint>
#include <string_view>

#include <imgui.h>
#include <d3d9.h>

class Image {
public:
    Image() = default;
    Image(const Image& other);
    Image(Image&& other) noexcept;
    Image(IDirect3DDevice9* device, const uint8_t* data, size_t length);
    Image(IDirect3DDevice9* device, std::string_view fileName);
    virtual ~Image();

    auto isValid() const {
        return m_texture != nullptr;
    }

    auto getTexture() {
        return m_texture;
    }
    
    auto getW() const {
        return m_w;
    }

    auto getH() const {
        return m_h;
    }

    operator ImTextureID() {
        return (ImTextureID)m_texture;
    }

    Image& operator=(const Image& other);
    Image& operator=(Image&& other) noexcept;

private:
    IDirect3DTexture9* m_texture{};
    int m_w{};
    int m_h{};

    void create(IDirect3DDevice9* device, const uint8_t* data, int w, int h);
};
