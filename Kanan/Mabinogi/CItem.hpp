#pragma once
class CString;
#pragma pack(push, 1)
class CItem {
public:
    struct SItemDBDesc {
        char pad_0[0x4];
        CString* type; // 0x4
        char pad_8[0x2c];
        CString* displayName; // 0x34
        char pad_38[0x4];
        CString* name; // 0x3c
        CString* description; // 0x40
        char pad_44[0x88];
        uint16_t maxStackCount; // 0xcc
    }; // Size: 0xce

    char pad_0[0x4];
    uint64_t* entityID; // 0x4
    uint32_t inventoryID; // 0x8
    uint32_t itemID; // 0xc
    uint32_t color1; // 0x10
    uint32_t color2; // 0x14
    uint32_t color3; // 0x18
    char pad_1c[0x4];
    uint32_t color4; // 0x20
    uint32_t color5; // 0x24
    uint32_t color6; // 0x28
    uint32_t stackCount; // 0x2c
    char pad_30[0x4];
    uint32_t positionX; // 0x34
    uint32_t positionY; // 0x38
    char pad_3c[0x28];
    uint32_t price; // 0x64
    uint32_t sellPrice; // 0x68
    char pad_6c[0x4];
    uint32_t durability; // 0x70
    uint32_t maxDurability; // 0x74
    uint32_t originalDurability; // 0x78
    char pad_7c[0x5c];
    SItemDBDesc* dbDesc; // 0xd8
    char pad_dc[0x1c];
    uint64_t ownerID; // 0xf8
    char pad_100[0x24];
    CString* name; // 0x124
}; // Size: 0x128
#pragma pack(pop)
