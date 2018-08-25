#pragma once

#include <cstdint>
#include <optional>
#include <type_traits>
#include <vector>

#include <intrin.h>

namespace kanan {
    enum class MessageElementType : uint8_t {
        NONE = 0,
        BYTE = 1,
        SHORT = 2,
        INT = 3,
        LONG = 4,
        FLOAT = 5,
        STRING = 6,
        BIN = 7
    };

    // A non-owning view of a message.
    class MessageView {
    public:
        MessageView() = delete;
        MessageView(const MessageView& other) = delete;
        MessageView(MessageView&& other) = delete;
        ~MessageView() = default;
        MessageView& operator=(const MessageView& other) = delete;
        MessageView& operator=(MessageView&& other) = delete;

        MessageView(const std::vector<uint8_t>& buffer);

        MessageElementType peek();
        bool nextIs(MessageElementType type);

        template<typename T>
        static constexpr MessageElementType getElementType() {
            if constexpr (std::is_integral_v<T>) {
                if constexpr (sizeof(T) == 1) {
                    return MessageElementType::BYTE;
                }
                else if constexpr (sizeof(T) == 2) {
                    return MessageElementType::SHORT;
                }
                else if constexpr (sizeof(T) == 4) {
                    return MessageElementType::INT;
                }
                else if constexpr (sizeof(T) == 8) {
                    return MessageElementType::LONG;
                }
                else {
                    return MessageElementType::NONE;
                }
            }
            else if constexpr (std::is_floating_point_v<T>) {
                return MessageElementType::FLOAT;
            }
            else if constexpr (std::is_same_v<std::decay_t<T>, char*>) {
                return MessageElementType::STRING;
            }
            else if constexpr (std::is_same_v<std::decay_t<T>, uint8_t*>) {
                return MessageElementType::BIN;
            }
            else {
                return MessageElementType::NONE;
            }
        }

        template<typename T>
        std::optional<T> get() {
            static_assert(std::is_arithmetic_v<T> ||
                std::is_same_v<T, std::string> ||
                std::is_same_v<T, std::vector<uint8_t>>,
                "MessageView::get() - Invalid type");

            if constexpr (std::is_arithmetic_v<T>) {
                if (!nextIs(getElementType<T>())) {
                    return {};
                }

                if (!isReadable(1 + sizeof(T))) {
                    return {};
                }

                m_ptr += 1;

                auto val = *(T*)(m_buffer.data() + m_ptr);

                m_ptr += sizeof(T);

                if constexpr (std::is_integral_v<T> && sizeof(T) > 1) {
                    val = swapEndian(val);
                }

                return val;
            }
            else if constexpr (std::is_same_v<T, std::string>) {
                if (!nextIs(MessageElementType::STRING)) {
                    return {};
                }

                if (!isReadable(1 + sizeof(uint16_t))) {
                    return {};
                }

                m_ptr += 1;

                const auto len = swapEndian(*(uint16_t*)&m_buffer[m_ptr]);

                m_ptr += sizeof(uint16_t);

                if (!isReadable(len)) {
                    return {};
                }

                std::string val{};

                copy_n(m_buffer.begin() + m_ptr, len - 1, back_inserter(val));

                m_ptr += len;

                return val;
            }
            else {
                if (!nextIs(MessageElementType::BIN)) {
                    return {};
                }

                if (!isReadable(1 + sizeof(uint16_t))) {
                    return {};
                }

                m_ptr += 1;

                const auto len = swapEndian(*(uint16_t*)&m_buffer[m_ptr]);

                m_ptr += sizeof(uint16_t);

                if (!isReadable(len)) {
                    return {};
                };

                std::vector<uint8_t> val{};

                val.resize(len);

                copy_n(m_buffer.begin() + m_ptr, len, val.begin());

                m_ptr += len;

                return val;
            }
        }

        static int readLeb128(const std::vector<uint8_t>& buffer, int& ptr);
        bool operator==(const MessageView& other);

        bool isReadable(size_t size);
        void skip(int num = 1);

        size_t getSize() const;

        void rewind();

        auto offset() const {
            return m_ptr;
        }

        auto bodyStart() const {
            return m_bodyStart;
        }

        auto numElements() const {
            return m_numElements;
        }

        auto bodyLen() const {
            return m_bodyLen;
        }

        auto op() const {
            return m_op;
        }

        auto id() const {
            return m_id;
        }

    private:
        const std::vector<uint8_t>& m_buffer;
        int m_ptr;
        int m_bodyStart;
        int m_numElements;
        int m_bodyLen;
        uint32_t m_op;
        uint64_t m_id;

        template<typename T>
        T swapEndian(T x) {
            static_assert(sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8, "Type must be 2, 4 or 8 bytes to swap endianess");

            if constexpr (sizeof(T) == 2) {
                return (T)_byteswap_ushort((unsigned short)x);
            }
            else if constexpr (sizeof(T) == 4) {
                return (T)_byteswap_ulong((unsigned long)x);
            }
            else {
                return (T)_byteswap_uint64((unsigned long long)x);
            }
        }
    };
}