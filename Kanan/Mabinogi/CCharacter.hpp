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
            private: char pad_0[0x8]; public:
            CString* value; // 0x8
            private: char pad_10[0x10]; public:
        }; // Size: 0x20

        class CMemberChar {
        public:
            private: char pad_0[0x8]; public:
            int8_t value; // 0x8
            private: char pad_9[0xf]; public:
        }; // Size: 0x18

        class CMemberUChar {
        public:
            private: char pad_0[0x8]; public:
            uint8_t value; // 0x8
            private: char pad_9[0xf]; public:
        }; // Size: 0x18

        class CMemberUShort {
        public:
            private: char pad_0[0x8]; public:
            uint16_t value; // 0x8
            private: char pad_a[0xe]; public:
        }; // Size: 0x18

        class CMemberULong {
        public:
            private: char pad_0[0x8]; public:
            uint32_t value; // 0x8
            private: char pad_c[0xc]; public:
        }; // Size: 0x18

        class CMemberUInt64 {
        public:
            private: char pad_0[0x8]; public:
            uint64_t value; // 0x8
            private: char pad_10[0x10]; public:
        }; // Size: 0x20

        class CMemberFloat {
        public:
            private: char pad_0[0x8]; public:
            float value; // 0x8
            private: char pad_c[0xc]; public:
        }; // Size: 0x18

        private: char pad_0[0x30]; public:
        CMemberString name; // 0x30
        CMemberString title; // 0x50
        CMemberString engTitle; // 0x70
        CMemberULong type; // 0x90
        CMemberUChar skinColor; // 0xa8
        CMemberUShort eyeType; // 0xc0
        CMemberUChar eyeColor; // 0xd8
        CMemberUChar mouthType; // 0xf0
        CMemberULong status; // 0x108
        CMemberULong statusEx; // 0x120
        CMemberULong statusEx2; // 0x138
        CMemberFloat scaleHeight; // 0x150
        CMemberFloat scaleFatness; // 0x168
        CMemberFloat scaleUpper; // 0x180
        CMemberFloat scaleLower; // 0x198
        CMemberULong regionID; // 0x1b0
        CMemberULong positionX; // 0x1c8
        CMemberULong positionY; // 0x1e0
        CMemberChar direction; // 0x1f8
        CMemberULong battleState; // 0x210
        CMemberUChar weaponSet; // 0x228
        CMemberULong extra1; // 0x240
        CMemberULong extra2; // 0x258
        CMemberULong extra3; // 0x270
        CMemberFloat combatPower; // 0x288
        CMemberString motionType; // 0x2a0
        CMemberUChar oddEyeLeftColor; // 0x2c0
        CMemberUChar oddEyeRightColor; // 0x2d8
        CMemberFloat life; // 0x2f0
        CMemberFloat lifeMaxBase; // 0x308
        CMemberFloat lifeDamaged; // 0x320
        CMemberFloat lifeMaxMod; // 0x338
        CMemberFloat mana; // 0x350
        CMemberFloat manaMaxBase; // 0x368
        CMemberFloat manaMaxMod; // 0x380
        CMemberFloat stamina; // 0x398
        CMemberFloat staminaMaxBase; // 0x3b0
        CMemberFloat staminaMaxMod; // 0x3c8
        CMemberFloat food; // 0x3e0
        CMemberFloat foodMinRatio; // 0x3f8
        CMemberUShort level; // 0x410
        CMemberULong cumulatedLevel; // 0x428
        CMemberUShort maxLevel; // 0x440
        CMemberUShort rebirthCount; // 0x458
        CMemberUShort lifeTimeSkill; // 0x470
        CMemberUInt64 experienceInt; // 0x488
        CMemberUShort age; // 0x4a8
        private: char pad_4c0[0xd60]; public:
        CMemberFloat dorcha; // 0x1220
        CMemberFloat dorchaMaxBase; // 0x1238
        CMemberFloat tuairim; // 0x1250
        CMemberFloat tuairimMaxBase; // 0x1268
    }; // Size: 0x1280

    class CAction {
    public:
        class CAnimatorContext {
        public:
            class CFramework {
            public:
                private: char pad_0[0xd0]; public:
                Vector3 position; // 0xd0
                private: char pad_dc[0x124]; public:
            }; // Size: 0x200

            private: char pad_0[0x8]; public:
            CFramework* framework; // 0x8
        }; // Size: 0x10

        private: char pad_0[0x48]; public:
        CCharacter* character; // 0x48
        CAnimatorContext* animatorContext; // 0x50
    }; // Size: 0x58

    class CSkillMgr {
    public:
        class CActiveSkill {
        public:
            private: char pad_0[0x18]; public:
            uint16_t skillID; // 0x18
            uint8_t charges; // 0x1a
            private: char pad_1b[0x9]; public:
            uint8_t state; // 0x24
        }; // Size: 0x25

        private: char pad_0[0x8]; public:
        CActiveSkill* skillInfo; // 0x8
    }; // Size: 0x10

    class CConditionMgr {
    public:
        class CCondition {
        public:
            private: char pad_0[0x4]; public:
            int32_t statusCount; // 0x4
            private: char pad_8[0x1d]; public:
        }; // Size: 0x25

        private: char pad_0[0x10]; public:
        CCondition* condition; // 0x10
        private: char pad_18[0x38]; public:
    }; // Size: 0x50

    class CSummonMaster {
    public:
        private: char pad_0[0x28]; public:
        uint64_t slaveEntityID; // 0x28
        private: char pad_30[0x8]; public:
    }; // Size: 0x38

    class CEquipmentItemInfo {
    public:
        uint32_t classID; // 0x0
        private: char pad_4[0xc]; public:
        uint32_t color1; // 0x10
        uint32_t color2; // 0x14
        uint32_t color3; // 0x18
        private: char pad_1c[0x14]; public:
        uint32_t color4; // 0x30
        uint32_t color5; // 0x34
        uint32_t color6; // 0x38
        private: char pad_3c[0x1c]; public:
    }; // Size: 0x58

    class CEquipment {
    public:
        private: char pad_0[0x6c]; public:
        CCharacter::CEquipmentItemInfo itemInfo[20]; // 0x6c
    }; // Size: 0x74c

    private: char pad_0[0x8]; public:
    uint64_t* entityID; // 0x8
    private: char pad_10[0xc8]; public:
    CParameter* parameter; // 0xd8
    CAction* action; // 0xe0
    CSkillMgr* skill; // 0xe8
    private: char pad_f0[0x20]; public:
    CConditionMgr* condition; // 0x110
    private: char pad_118[0x80]; public:
    CSummonMaster* summonMaster; // 0x198
    private: char pad_1a0[0x2e8]; public:
    CEquipment* equipment; // 0x488
    private: char pad_490[0xa8]; public:
    uint64_t targetID; // 0x538
    private: char pad_540[0x8]; public:
    uint64_t mouseTarget; // 0x548
}; // Size: 0x550
#pragma pack(pop)
