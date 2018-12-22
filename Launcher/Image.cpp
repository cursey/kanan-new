#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Image.hpp"

using std::runtime_error;
using std::string_view;

Image::Image(const Image& other)
    : m_texture{ other.m_texture },
    m_w{ other.m_w },
    m_h{ other.m_h }
{
    if (m_texture != nullptr) {
        m_texture->AddRef();
    }
}

Image::Image(Image&& other) noexcept
    : m_texture{ other.m_texture },
    m_w{ other.m_w },
    m_h{ other.m_h }
{
    other.m_texture = nullptr;
}

Image::Image(IDirect3DDevice9* device, const uint8_t* data, size_t length)
    : m_texture{},
    m_w{},
    m_h{}
{
    // Load the image data.
    int w{}, h{}, n{};
    auto img = stbi_load_from_memory(data, length, &w, &h, &n, 4);

    if (img == nullptr) {
        throw runtime_error{ "Failed to load image from memory" };
    }

    create(device, img, w, h);

    // Cleanup the image data since we don't need it after uploading it to the
    // texture.
    stbi_image_free(img);
}

Image::Image(IDirect3DDevice9* device, string_view fileName)
    : m_texture{},
    m_w{},
    m_h{}
{
    // Load the image data.
    int w{}, h{}, n{};
    auto img = stbi_load(fileName.data(), &w, &h, &n, 4);

    if (img == nullptr) {
        throw runtime_error{ "Failed to load image from file" };
    }

    create(device, img, w, h);

    // Cleanup the image data since we don't need it after uploading it to the
    // texture.
    stbi_image_free(img);
}

Image::~Image() {
    if (m_texture) {
        m_texture->Release();
        m_texture = nullptr;
    }
}

Image& Image::operator=(const Image& other) {
    m_texture = other.m_texture;
    m_w = other.m_w;
    m_h = other.m_h;

    if (m_texture != nullptr) {
        m_texture->AddRef();
    }

    return *this;
}

Image& Image::operator=(Image&& other) noexcept {
    m_texture = other.m_texture;
    m_w = other.m_w;
    m_h = other.m_h;
    other.m_texture = nullptr;

    return *this;
}

void Image::create(IDirect3DDevice9 * device, const uint8_t* data, int w, int h) {
    // Create the texture.
    if (FAILED(device->CreateTexture(w, h, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_texture, nullptr))) {
        throw runtime_error{ "Failed to create Direct3D texture" };
    }

    // Upload the image date to the texture.
    D3DLOCKED_RECT rect{};

    if (FAILED(m_texture->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD))) {
        throw runtime_error{ "Failed to lock the texture for writing" };
    }

    auto dst = (int*)rect.pBits;
    auto pitch = rect.Pitch / sizeof(int);
    auto src = (int*)data;

    for (auto y = 0; y < h; ++y) {
        for (auto x = 0; x < w; ++x) {
            // Copy a pixel to the correct location on the texture with regard to
            // the textures pitch and image format.
            auto p = src[y * w + x];
            auto r = (p & 0xFF);
            auto g = (p & 0xFF00) >> 8;
            auto b = (p & 0xFF0000) >> 16;
            auto a = (p & 0xFF000000) >> 24;

            dst[y * pitch + x] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }

    if (FAILED(m_texture->UnlockRect(0))) {
        throw runtime_error{ "Failed to unlock the texture after writing" };
    }

    m_w = w;
    m_h = h;
}
