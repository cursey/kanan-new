#pragma once

#include <cstdint>
#include <string>
#include <optional>

#include "Mabinogi.hpp"

namespace kanan {
    class KCharacter : public CCharacter {
    public:
        std::optional<uint64_t> getID() const;
        std::optional<std::string> getName() const;
        std::optional<Vector3> getPosition() const;
        std::optional<bool> isDead() const;
    };
}
