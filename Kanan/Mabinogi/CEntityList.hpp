#pragma once
class CCharacter;
class CItem;
#pragma pack(push, 1)
class CEntityList {
public:
    class CItemList {
    public:
        class CCItemListNode {
        public:
        }; // Size: 0x0

        class CItemListNode {
        public:
            class CItemListNodeEntry {
            public:
                char pad_0[0x10];
                CItem* item; // 0x10
            }; // Size: 0x14

            CItemListNodeEntry* entry; // 0x0
            CEntityList::CItemList::CItemListNode* next; // 0x4
        }; // Size: 0x8

        char pad_0[0x4];
        CItemListNode* root; // 0x4
        uint32_t count; // 0x8
    }; // Size: 0xc

    class CCharacterList {
    public:
        class CCharacterListNode {
        public:
            class CCharacterListNodeEntry {
            public:
                char pad_0[0x10];
                CCharacter* character; // 0x10
            }; // Size: 0x14

            CCharacterListNodeEntry* entry; // 0x0
            CEntityList::CCharacterList::CCharacterListNode* next; // 0x4
        }; // Size: 0x8

        char pad_0[0x4];
        CCharacterListNode* root; // 0x4
        uint32_t count; // 0x8
    }; // Size: 0xc

    char pad_0[0x8];
    CItemList items; // 0x8
    char pad_14[0x14];
    CCharacterList characters; // 0x28
}; // Size: 0x34
#pragma pack(pop)
