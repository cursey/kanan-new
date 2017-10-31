#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace kanan {
    class Pattern {
    public:
        Pattern() = delete;
        Pattern(const Pattern& other) = default;
        Pattern(Pattern&& other) = default;
        Pattern(std::string pattern);
        ~Pattern() = default;

        std::optional<uintptr_t> find(uintptr_t start, size_t length);

        Pattern& operator=(const Pattern& other) = default;
        Pattern& operator=(Pattern&& other) = default;

    private:
        std::vector<int> m_pattern;
    };
}
