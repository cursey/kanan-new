#pragma once
#pragma pack(push, 1)
class CWorld {
public:
    char pad_0[0x160];
    uint64_t localPlayerID; // 0x160
    char pad_168[0x98];
}; // Size: 0x200
#pragma pack(pop)
