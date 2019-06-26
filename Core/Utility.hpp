#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace kanan {
    bool isKeyDown(int key);
    bool wasKeyPressed(int key);

    std::string hexify(const uint8_t* data, size_t length);
    std::string hexify(const std::vector<uint8_t>& data);
}
