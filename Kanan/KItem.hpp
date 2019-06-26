#pragma once

#include <cstdint>
#include <optional>

#include "Mabinogi.hpp"

namespace kanan {
    class KItem : public CItem {
    public:
        std::optional<uint64_t> getID() const;
        std::optional<std::string> getName() const;
        std::optional<uint16_t> getMaxStackCount() const;
    };
}
