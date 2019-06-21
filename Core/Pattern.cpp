#include <algorithm>

#include <Windows.h>

#include "Memory.hpp"
#include "Pattern.hpp"

using namespace std;

namespace kanan {
    static uint8_t toByte(char digit) {
        if (digit >= '0' && digit <= '9') {
            return (digit - '0');
        }

        if (digit >= 'a' && digit <= 'f') {
            return (digit - 'a' + 10);
        }

        if (digit >= 'A' && digit <= 'F') {
            return (digit - 'A' + 10);
        }

        return 0;
    }

    Pattern::Pattern(const string& pattern)
        : m_pattern{}
    {
        m_pattern = move(buildPattern(pattern));
    }

    optional<uintptr_t> Pattern::find(uintptr_t start, size_t length) {
        auto patternLength = m_pattern.size();
        auto end = start + length - patternLength;
        auto i = start;

        // Do we start at a readable address? If not, align to the next page.
        if (!isGoodCodePtr(i, patternLength)) {
            i = ((i + 0x1000 - 1) / 0x1000) * 0x1000;
        }

        while (i <= end) {
            // If we're at the start of a new page, check to see if its a readable 
            // address. If not, skip an entire page.
            if ((i % 0x1000) == 0 && !isGoodCodePtr(i, patternLength)) {
                i += 0x1000;
                continue;
            }

            // Test the pattern at this address.
            auto j = i;
            auto failedToMatch = false;

            for (auto& k : m_pattern) {
                if (k != -1 && k != *(uint8_t*)j) {
                    failedToMatch = true;
                    break;
                }

                ++j;
            }

            // If we didn't fail to match, then we found a match so return it.
            if (!failedToMatch) {
                return i;
            }

            // Otherwise, advance a byte.
            ++i;
        }

        // No match found.
        return nullopt;
    }

    vector<int16_t> buildPattern(string patternStr) {
        // Remove spaces from the pattern string.
        patternStr.erase(remove_if(begin(patternStr), end(patternStr), isspace), end(patternStr));

        auto length = patternStr.length();
        vector<int16_t> pattern{};

        for (size_t i = 0; i < length;) {
            auto p1 = patternStr[i];

            if (p1 != '?') {
                // Bytes require 2 hex characters to encode, make sure we don't read
                // past the end of the pattern string attempting to read the next char.
                if (i + 1 >= length) {
                    break;
                }

                auto p2 = patternStr[i + 1];
                auto value = toByte(p1) << 4 | toByte(p2);

                pattern.emplace_back(value);

                i += 2;
            }
            else {
                // Wildcard's (?'s) get encoded as a -1.
                pattern.emplace_back(-1);
                i += 1;
            }
        }

        return pattern;
    }
}
