#pragma once
#pragma pack(push, 1)
class CString {
public:
    char pad_0[0x4];
    int32_t referenceCount; // 0x4
    char pad_8[0x4];
    uint32_t length; // 0xc
    uint32_t capacity; // 0x10
    char pad_14[0x8];
    // Metadata: utf16*
    char16_t buffer[70]; // 0x1c
}; // Size: 0xa8
#pragma pack(pop)
