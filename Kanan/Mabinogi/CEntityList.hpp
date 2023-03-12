#pragma once
class CItem;
class CCharacter;
#pragma pack(push, 1)
class CEntityList {
public:
    class CItemList {
    public:
        class CItemListNode {
        public:
            CEntityList::CItemList::CItemListNode* next; // 0x0
            char pad_8[0x10];
            CItem* item; // 0x18
        }; // Size: 0x20

        char pad_0[0x10];
        CItemListNode** root; // 0x10
        uint32_t count; // 0x18
    }; // Size: 0x1c

    class CCharacterList {
    public:
        class CCharacterListNode {
        public:
            CEntityList::CCharacterList::CCharacterListNode* next; // 0x0
            char pad_8[0x10];
            CCharacter* character; // 0x18
            char pad_20[0x8];
        }; // Size: 0x28

        char pad_0[0x10];
        CCharacterListNode** root; // 0x10
        uint32_t count; // 0x18
    }; // Size: 0x1c

    char pad_0[0x8];
    CItemList items; // 0x8
    char pad_24[0x24];
    CCharacterList characters; // 0x48
    char pad_64[0x9c];
}; // Size: 0x100
#pragma pack(pop)
