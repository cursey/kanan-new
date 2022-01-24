#include <String.hpp>

#include "KCharacter.hpp"

using namespace std;

namespace kanan {
    optional<uint64_t> KCharacter::getID() const {
        if (entityID == nullptr) {
            return {};
        }

        return *entityID;
    }

    optional<std::string> KCharacter::getName() const {
        if (parameter == nullptr) {
            return {};
        }

        auto name = parameter->name.value;

        if (name == nullptr) {
            return {};
        }

        return narrow((wchar_t*)&name->buffer[0]);
    }

    optional<Vector3> KCharacter::getPosition() const {
        if (action == nullptr) {
            return {};
        }

        auto context = action->animatorContext;

        if (context == nullptr) {
            return {};
        }

        auto framework = context->framework;

        if (framework == nullptr) {
            return {};
        }

        return framework->position;
    }

    optional<bool> KCharacter::isDead() const {
        if (parameter == nullptr) {
            return {};
        }

        return (parameter->status.value >> 1) & 1;
    }
}
