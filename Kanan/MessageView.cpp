#include <algorithm>

#include "MessageView.hpp"

using namespace std;

namespace kanan {
    MessageView::MessageView(const vector<uint8_t>& buffer)
        : m_buffer{ buffer },
        m_ptr{ 12 },
        m_bodyStart{},
        m_numElements{},
        m_bodyLen{},
        m_op{},
        m_id{}
    {
        m_op = swapEndian(*(uint32_t*)(m_buffer.data()));
        m_id = swapEndian(*(uint64_t*)(m_buffer.data() + sizeof(uint32_t)));
        m_bodyLen = readLeb128(m_buffer, m_ptr);
        m_numElements = readLeb128(m_buffer, m_ptr);

        m_ptr++; // 0x00

        m_bodyStart = m_ptr;
    }

    MessageElementType MessageView::peek() {
        if ((size_t)m_ptr + 2 > m_buffer.size()) {
            return MessageElementType::NONE;
        }

        return (MessageElementType)m_buffer[m_ptr];
    }

    bool MessageView::nextIs(MessageElementType type) {
        return peek() == type;
    }

    int MessageView::readLeb128(const vector<uint8_t>& buffer, int& ptr) {
        auto result = 0;

        for (auto i = 0; ; ++i) {
            result |= (buffer[ptr] & 0x7F) << (i * 7);

            if ((buffer[ptr++] & 0x80) == 0) {
                break;
            }
        }

        return result;
    }

    bool MessageView::isReadable(size_t size) {
        return m_ptr + size <= m_buffer.size();
    }

    void MessageView::skip(int num) {
        for (auto i = 0; i < num; ++i) {
            switch (peek()) {
            case MessageElementType::BYTE: get<uint8_t>(); break;
            case MessageElementType::SHORT: get<uint16_t>(); break;
            case MessageElementType::INT: get<uint32_t>(); break;
            case MessageElementType::LONG: get<uint64_t>(); break;
            case MessageElementType::FLOAT: get<float>(); break;
            case MessageElementType::STRING: get<string>(); break;
            case MessageElementType::BIN: get<vector<uint8_t>>(); break;
            default: break;
            }
        }
    }

    size_t MessageView::getSize() const {
        size_t i{ 4 + 8 };

        auto n = m_bodyLen;
        do { ++i; n >>= 7; } while (n != 0);

        n = m_numElements;
        do { ++i; n >>= 7; } while (n != 0);

        ++i;
        i += m_bodyLen;

        return i;
    }

    void MessageView::rewind() {
        m_ptr = m_bodyStart;
    }

    bool MessageView::operator==(const MessageView& other) {
        return op() == other.op() &&
            id() == other.id() &&
            equal(
                m_buffer.begin() + m_bodyStart, m_buffer.begin() + m_bodyStart + m_bodyLen,
                other.m_buffer.begin() + other.m_bodyStart, other.m_buffer.begin() + other.m_bodyStart + other.m_bodyLen
            );
    }
}
