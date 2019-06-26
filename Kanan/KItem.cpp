#include <String.hpp>

#include "KItem.hpp"

using namespace std;

namespace kanan {
    optional<uint64_t> KItem::getID() const {
        if (entityID == nullptr) {
            return {};
        }

        return entityID->id;
    }

    optional<std::string> KItem::getName() const {
        if (name == nullptr) {
            return {};
        }

        return narrow(name->buffer);
    }

    optional<uint16_t> KItem::getMaxStackCount() const {
        if (dbDesc == nullptr) {
            return {};
        }

        return dbDesc->maxStackCount;
	}
}
