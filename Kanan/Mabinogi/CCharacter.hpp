#pragma once
#include "Vector3.hpp"
class CString;
#pragma pack(push, 1)
class CCharacter {
public:
    class CParameter {
    public:
        class CMemberString {
        public:
            char pad_0[0x4];
            CString* value; // 0x4
            char pad_8[0x8];
        }; // Size: 0x10

        class CMemberChar {
        public:
            char pad_0[0x4];
            int8_t value; // 0x4
            char pad_5[0x7];
        }; // Size: 0xc

        class CMemberUChar {
        public:
            char pad_0[0x4];
            uint8_t value; // 0x4
            char pad_5[0x7];
        }; // Size: 0xc

        class CMemberUShort {
        public:
            char pad_0[0x4];
            uint16_t value; // 0x4
            char pad_6[0x6];
        }; // Size: 0xc

        class CMemberULong {
        public:
            char pad_0[0x4];
            uint32_t value; // 0x4
            char pad_8[0x8];
        }; // Size: 0x10

        class CMemberUInt64 {
        public:
            char pad_0[0x4];
            uint64_t value; // 0x4
            char pad_c[0xc];
        }; // Size: 0x18

        class CMemberFloat {
        public:
            char pad_0[0x4];
            float value; // 0x4
            char pad_8[0x8];
        }; // Size: 0x10

        char pad_0[0x20];
        CMemberString name; // 0x20
        CMemberString title; // 0x30
        CMemberString engTitle; // 0x40
        CMemberULong type; // 0x50
        CMemberUChar skinColor; // 0x60
        CMemberUShort eyeType; // 0x6c
        CMemberUChar eyeColor; // 0x78
        CMemberUChar mouthType; // 0x84
        CMemberULong status; // 0x90
        CMemberULong statusEx; // 0xa0
        CMemberULong statusEx2; // 0xb0
        CMemberFloat scaleHeight; // 0xc0
        CMemberFloat scaleFatness; // 0xd0
        CMemberFloat scaleUpper; // 0xe0
        CMemberFloat scaleLower; // 0xf0
        CMemberULong regionID; // 0x100
        CMemberULong positionX; // 0x110
        CMemberULong positionY; // 0x120
        CMemberChar direction; // 0x130
        CMemberULong battleState; // 0x13c
        CMemberUChar weaponSet; // 0x14c
        CMemberULong extra1; // 0x158
        CMemberULong extra2; // 0x168
        CMemberULong extra3; // 0x178
        CMemberFloat combatPower; // 0x188
        CMemberString motionType; // 0x198
        CMemberUChar oddEyeLeftColor; // 0x1a8
        CMemberUChar oddEyeRightColor; // 0x1b4
        CMemberFloat life; // 0x1c0
        CMemberFloat lifeMaxBase; // 0x1d0
        CMemberFloat lifeDamaged; // 0x1e0
        CMemberFloat lifeMaxMod; // 0x1f0
        CMemberFloat mana; // 0x200
        CMemberFloat manaMaxBase; // 0x210
        CMemberFloat manaMaxMod; // 0x220
        CMemberFloat stamina; // 0x230
        CMemberFloat staminaMaxBase; // 0x240
        CMemberFloat staminaMaxMod; // 0x250
        CMemberFloat food; // 0x260
        CMemberFloat foodMinRatio; // 0x270
        CMemberUShort level; // 0x280
        CMemberULong cumulatedLevel; // 0x28c
        CMemberUShort maxLevel; // 0x29c
        CMemberUShort rebirthCount; // 0x2a8
        CMemberUShort lifeTimeSkill; // 0x2b4
        CMemberUInt64 experienceInt; // 0x2c0
        CMemberUShort age; // 0x2d8
        char pad_2e4[0x798];
        CMemberFloat dorcha; // 0xa7c
        CMemberFloat dorchaMaxBase; // 0xa8c
        CMemberFloat tuairim; // 0xa9c
        CMemberFloat tuairimMaxBase; // 0xaac
    }; // Size: 0xabc

    class CAction {
    public:
        class CAnimatorContext {
        public:
            class CFramework {
            public:
                char pad_0[0xa0];
                Vector3 position; // 0xa0
            }; // Size: 0xac

            char pad_0[0x4];
            CFramework* framework; // 0x4
        }; // Size: 0x8

        char pad_0[0x40];
        CCharacter* character; // 0x40
        CAnimatorContext* animatorContext; // 0x44
    }; // Size: 0x48

    class CSkillMgr {
    public:
        class CActiveSkill {
        public:
            char pad_0[0x10];
            uint16_t skillID; // 0x10
            uint8_t charges; // 0x12
            char pad_13[0x9];
            uint8_t state; // 0x1c
        }; // Size: 0x1d

        char pad_0[0x4];
        CActiveSkill* skillInfo; // 0x4
    }; // Size: 0x8

    class CConditionMgr {
    public:
        class CCondition {
        public:
            char pad_0[0x4];
            int32_t statusCount; // 0x4
            char pad_8[0x1d];
        }; // Size: 0x25

        char pad_0[0x8];
        CCondition* condition; // 0x8
        char pad_c[0x44];
    }; // Size: 0x50

    class CEquipmentItemInfo {
    public:
        char pad_0[0xc];
        uint32_t classID; // 0xc
        char pad_10[0xc];
        uint32_t color1; // 0x1c
        uint32_t color2; // 0x20
        uint32_t color3; // 0x24
        char pad_28[0x4];
        uint32_t color4; // 0x2c
        uint32_t color5; // 0x30
        uint32_t color6; // 0x34
        char pad_38[0x18];
    }; // Size: 0x50

    class CEquipment {
    public:
        CCharacter::CEquipmentItemInfo itemInfo[20]; // 0x0
    }; // Size: 0x640

    char pad_0[0x4];
    uint64_t* entityID; // 0x4
    char pad_8[0xa0];
    CParameter* parameter; // 0xa8
    CAction* action; // 0xac
    CSkillMgr* skill; // 0xb0
    char pad_b4[0x10];
    CConditionMgr* condition; // 0xc4
    char pad_c8[0x1b8];
    CEquipment* equipment; // 0x280
    char pad_284[0x54];
    uint64_t targetID; // 0x2d8
    char pad_2e0[0x8];
    uint64_t mouseTarget; // 0x2e8
}; // Size: 0x2f0
#pragma pack(pop)
