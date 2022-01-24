#pragma once
#pragma pack(push, 1)
class CWorld {
public:
    char pad_0[0xb8];
    uint64_t localPlayerID; // 0xb8
}; // Size: 0xc0
#pragma pack(pop)
