#pragma once

#include <array>
#include <cstdint>

//
// ReClass.NET generates code that expects these types to be defined. Instead of
// making more elaborate types for these, we just use an array for now.
//
using Vector2 = std::array<float, 2>;
using Vector3 = std::array<float, 3>;
using Vector4 = std::array<float, 4>;
using Matrix3x3 = std::array<float, 3 * 3>;
using Matrix3x4 = std::array<float, 3 * 4>;
using Matrix4x4 = std::array<float, 4 * 4>;

// Created with ReClass.NET by KN4CK3R

class CCameraState
{
public:
    char pad_0000[12]; //0x0000
    Vector3 target; //0x000C
    char pad_0018[4]; //0x0018
    Vector3 position; //0x001C
    char pad_0028[8]; //0x0028
    Vector4 forward; //0x0030
    char pad_0040[4]; //0x0040
    float zNear; //0x0044
    float zFar; //0x0048
    float fov; //0x004C
    float screenWidth; //0x0050
    float screenHeight; //0x0054
    char pad_0058[28]; //0x0058
    float aspectRatio; //0x0074
    char pad_0078[212]; //0x0078
    Matrix4x4 viewMatrix; //0x014C
    char pad_018C[84]; //0x018C
}; //Size: 0x01E0

class CCamera
{
public:
    class CCameraState* state; //0x0000
    char pad_0004[64]; //0x0004
}; //Size: 0x0044

class CRenderer
{
public:
    char pad_0000[36]; //0x0000
    class CCamera* camera; //0x0024
    char pad_0028[28]; //0x0028
}; //Size: 0x0044

class CRendererPtr
{
public:
    class CRenderer* renderer; //0x0000
    char pad_0004[60]; //0x0004
}; //Size: 0x0040

class CEntityID
{
public:
    uint64_t id; //0x0000
    char pad_0008[64]; //0x0008
}; //Size: 0x0048

class CString
{
public:
    char pad_0000[4]; //0x0000
    int32_t referenceCount; //0x0004
    char pad_0008[4]; //0x0008
    uint32_t length; //0x000C
    uint32_t capacity; //0x0010
    char pad_0014[8]; //0x0014
    wchar_t buffer[70]; //0x001C
    char pad_00A8[36]; //0x00A8
}; //Size: 0x00CC

class SItemDBDesc
{
public:
    char pad_0000[40]; //0x0000
    class CString* type; //0x0028
    char pad_002C[16]; //0x002C
    class CString* displayName; //0x003C
    char pad_0040[4]; //0x0040
    class CString* name; //0x0044
    class CString* description; //0x0048
    char pad_004C[244]; //0x004C
}; //Size: 0x0140

class CItem
{
public:
    char pad_0000[4]; //0x0000
    class CEntityID* entityID; //0x0004
    uint32_t inventoryID; //0x0008
    uint32_t itemID; //0x000C
    uint32_t color1; //0x0010
    uint32_t color2; //0x0014
    uint32_t color3; //0x0018
    uint32_t stackCount; //0x001C
    char pad_0020[4]; //0x0020
    uint32_t positionX; //0x0024
    uint32_t positionY; //0x0028
    char pad_002C[24]; //0x002C
    uint32_t price; //0x0044
    uint32_t sellPrice; //0x0048
    char pad_004C[8]; //0x004C
    uint32_t durability; //0x0054
    uint32_t maxDurability; //0x0058
    uint32_t originalDurability; //0x005C
    char pad_0060[92]; //0x0060
    class SItemDBDesc* dbDesc; //0x00BC
    char pad_00C0[32]; //0x00C0
    uint64_t ownerID; //0x00E0
    char pad_00E8[36]; //0x00E8
    class CString* name; //0x010C
    char pad_0110[56]; //0x0110
}; //Size: 0x0148

class CItemListNodeEntry
{
public:
    char pad_0000[16]; //0x0000
    class CItem* item; //0x0010
    char pad_0014[48]; //0x0014
}; //Size: 0x0044

class CItemListNode
{
public:
    class CItemListNodeEntry* entry; //0x0000
    class CItemListNode* next; //0x0004
}; //Size: 0x0008

class CItemList
{
public:
    char pad_0000[4]; //0x0000
    class CItemListNode* root; //0x0004
    uint32_t count; //0x0008
}; //Size: 0x000C

class CMemberString
{
public:
    char pad_0000[4]; //0x0000
    class CString* value; //0x0004
    char pad_0008[8]; //0x0008
}; //Size: 0x0010

class CMemberULong
{
public:
    char pad_0000[4]; //0x0000
    uint32_t value; //0x0004
    char pad_0008[8]; //0x0008
}; //Size: 0x0010

class CMemberUChar
{
public:
    char pad_0000[4]; //0x0000
    uint8_t value; //0x0004
    char pad_0005[7]; //0x0005
}; //Size: 0x000C

class CMemberUShort
{
public:
    char pad_0000[4]; //0x0000
    uint16_t value; //0x0004
    char pad_0006[6]; //0x0006
}; //Size: 0x000C

class CMemberFloat
{
public:
    char pad_0000[4]; //0x0000
    float value; //0x0004
    char pad_0008[8]; //0x0008
}; //Size: 0x0010

class CMmemberChar
{
public:
    char pad_0000[4]; //0x0000
    int8_t value; //0x0004
    char pad_0005[7]; //0x0005
}; //Size: 0x000C

class CMemberUInt64
{
public:
    char pad_0000[8]; //0x0000
    uint64_t value; //0x0008
    char pad_0010[8]; //0x0010
}; //Size: 0x0018

class CParameter
{
public:
    char pad_0000[28]; //0x0000
    CMemberString name; //0x001C
    CMemberString title; //0x002C
    CMemberString engTitle; //0x003C
    CMemberULong type; //0x004C
    CMemberUChar skinColor; //0x005C
    CMemberUShort eyeType; //0x0068
    CMemberUChar eyeColor; //0x0074
    CMemberUChar mouthType; //0x0080
    CMemberULong status; //0x008C
    CMemberULong statusEx; //0x009C
    CMemberULong statusEx2; //0x00AC
    CMemberFloat scaleHeight; //0x00BC
    CMemberFloat scaleFatness; //0x00CC
    CMemberFloat scaleUpper; //0x00DC
    CMemberFloat scaleLower; //0x00EC
    CMemberULong regionID; //0x00FC
    CMemberULong positionX; //0x010C
    CMemberULong positionY; //0x011C
    CMmemberChar direction; //0x012C
    CMemberULong battleState; //0x0138
    CMemberUChar weaponSet; //0x0148
    CMemberULong extra1; //0x0154
    CMemberULong extra2; //0x0164
    CMemberULong extra3; //0x0174
    CMemberFloat combatPower; //0x0184
    CMemberString motionType; //0x0194
    CMemberUChar oddEyeLeftColor; //0x01A4
    CMemberUChar oddEyeRightColor; //0x01B0
    CMemberFloat life; //0x01BC
    CMemberFloat lifeMaxBase; //0x01CC
    CMemberFloat lifeDamaged; //0x01DC
    CMemberFloat lifeMaxMod; //0x01EC
    CMemberFloat mana; //0x01FC
    CMemberFloat manaMaxBase; //0x020C
    CMemberFloat manaMaxMod; //0x021C
    CMemberFloat stamina; //0x022C
    CMemberFloat staminaMaxBase; //0x023C
    CMemberFloat staminaMaxMod; //0x024C
    CMemberFloat food; //0x025C
    CMemberFloat foodMinRatio; //0x026C
    CMemberUShort level; //0x027C
    CMemberULong cumulatedLevel; //0x0288
    CMemberUShort maxLevel; //0x0298
    CMemberUShort rebirthCount; //0x02A4
    CMemberUShort lifeTimeSkill; //0x02B0
    char pad_02BC[4]; //0x02BC
    CMemberUInt64 experienceInt; //0x02C0
    CMemberUShort age; //0x02D8
    char pad_02E4[1148]; //0x02E4
}; //Size: 0x0760

class CFramework
{
public:
    char pad_0000[152]; //0x0000
    Vector3 position; //0x0098
    char pad_00A4[168]; //0x00A4
}; //Size: 0x014C

class CAnimatorContext
{
public:
    char pad_0000[4]; //0x0000
    class CFramework* framework; //0x0004
    char pad_0008[60]; //0x0008
}; //Size: 0x0044

class CAction
{
public:
    char pad_0000[64]; //0x0000
    class CCharacter* character; //0x0040
    class CAnimatorContext* animatorContext; //0x0044
    char pad_0048[1020]; //0x0048
}; //Size: 0x0444

class CEquipmentItemInfo
{
public:
    char pad_0000[28]; //0x0000
    uint32_t color1; //0x001C
    uint32_t color2; //0x0020
    uint32_t color3; //0x0024
    char pad_0028[12]; //0x0028
}; //Size: 0x0034

class CEquipment
{
public:
    CEquipmentItemInfo itemInfo[19]; //0x0000
    char pad_03DC[384]; //0x03DC
}; //Size: 0x055C

class CCharacter
{
public:
    char pad_0000[4]; //0x0000
    class CEntityID* entityID; //0x0004
    char pad_0008[136]; //0x0008
    class CParameter* parameter; //0x0090
    class CAction* action; //0x0094
    char pad_0098[396]; //0x0098
    class CEquipment* equipment; //0x0224
    char pad_0228[104]; //0x0228
    uint64_t aggroTargetID; //0x0290
    char pad_0298[684]; //0x0298
}; //Size: 0x0544

class CCharacterListNodeEntry
{
public:
    char pad_0000[16]; //0x0000
    class CCharacter* character; //0x0010
    char pad_0014[48]; //0x0014
}; //Size: 0x0044

class CCharacterListNode
{
public:
    class CCharacterListNodeEntry* entry; //0x0000
    class CCharacterListNode* next; //0x0004
}; //Size: 0x0008

class CCharacterList
{
public:
    char pad_0000[4]; //0x0000
    class CCharacterListNode* root; //0x0004
    uint32_t count; //0x0008
}; //Size: 0x000C

class CEntityList
{
public:
    char pad_0000[8]; //0x0000
    CItemList items; //0x0008
    char pad_0014[32]; //0x0014
    CCharacterList characters; //0x0034
    char pad_0040[20]; //0x0040
}; //Size: 0x0054

class CEntityListPtr
{
public:
    class CEntityList* entityList; //0x0000
    char pad_0004[60]; //0x0004
}; //Size: 0x0040

class CWorld
{
public:
    char pad_0000[160]; //0x0000
    uint64_t localPlayerID; //0x00A0
    char pad_00A8[944]; //0x00A8
}; //Size: 0x0458

class CWorldPtr
{
public:
    class CWorld* world; //0x0000
    char pad_0004[60]; //0x0004
}; //Size: 0x0040

class CAccount
{
public:
    char pad_0000[68]; //0x0000
}; //Size: 0x0044

class CAccountPtr
{
public:
    class CAccount* account; //0x0000
    char pad_0004[60]; //0x0004
}; //Size: 0x0040