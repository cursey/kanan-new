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
using Vector2f = Vector2;
using Vector3f = Vector3;
using Vector4f = Vector4;
using Matrix3x3f = Matrix3x3;
using Matrix3x4f = Matrix3x4;
using Matrix4x4f = Matrix4x4;

// Created with ReClass.NET by KN4CK3R

class CCameraState
{
public:
    char pad_0000[4]; //0x0000
    Vector3f target; //0x0004
    char pad_0010[8]; //0x0010
    Vector3f position; //0x0018
    char pad_0024[8]; //0x0024
    Vector4f forward; //0x002C
    char pad_003C[4]; //0x003C
    float drawDistance; //0x0040
    float zNear; //0x0044
    float zFar; //0x0048
    float fov; //0x004C
    float screenWidth; //0x0050
    float screenHeight; //0x0054
    char pad_0058[28]; //0x0058
    float aspectRatio; //0x0074
    char pad_0078[212]; //0x0078
    Matrix4x4f viewMatrix; //0x014C
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
    char pad_00A8[32]; //0x00A8
}; //Size: 0x00C8

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
    uint32_t color4; //0x001C
    uint32_t color5; //0x0020
    uint32_t color6; //0x0024
    char pad_0028[4]; //0x0028
    uint32_t stackCount; //0x002C
    char pad_0030[4]; //0x0030
    uint32_t positionX; //0x0034
    uint32_t positionY; //0x0038
    char pad_003C[28]; //0x003C
    uint32_t price; //0x0058
    uint32_t sellPrice; //0x005C
    char pad_0060[4]; //0x0060
    uint32_t durability; //0x0064
    uint32_t maxDurability; //0x0068
    uint32_t originalDurability; //0x006C
    char pad_0070[92]; //0x0070
    class SItemDBDesc* dbDesc; //0x00CC
    char pad_00D0[32]; //0x00D0
    uint64_t ownerID; //0x00F0
    char pad_00F8[36]; //0x00F8
    class CString* name; //0x011C
    char pad_0120[56]; //0x0120
}; //Size: 0x0158

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
    char pad_0000[36]; //0x0000
    CMemberString name; //0x0024
    CMemberString title; //0x0034
    CMemberString engTitle; //0x0044
    CMemberULong type; //0x0054
    CMemberUChar skinColor; //0x0064
    CMemberUShort eyeType; //0x0070
    CMemberUChar eyeColor; //0x007C
    CMemberUChar mouthType; //0x0088
    CMemberULong status; //0x0094
    CMemberULong statusEx; //0x00A4
    CMemberULong statusEx2; //0x00B4
    CMemberFloat scaleHeight; //0x00C4
    CMemberFloat scaleFatness; //0x00D4
    CMemberFloat scaleUpper; //0x00E4
    CMemberFloat scaleLower; //0x00F4
    CMemberULong regionID; //0x0104
    CMemberULong positionX; //0x0114
    CMemberULong positionY; //0x0124
    CMmemberChar direction; //0x0134
    CMemberULong battleState; //0x0140
    CMemberUChar weaponSet; //0x0150
    CMemberULong extra1; //0x015C
    CMemberULong extra2; //0x016C
    CMemberULong extra3; //0x017C
    CMemberFloat combatPower; //0x018C
    CMemberString motionType; //0x019C
    CMemberUChar oddEyeLeftColor; //0x01AC
    CMemberUChar oddEyeRightColor; //0x01B8
    CMemberFloat life; //0x01C4
    CMemberFloat lifeMaxBase; //0x01D4
    CMemberFloat lifeDamaged; //0x01E4
    CMemberFloat lifeMaxMod; //0x01F4
    CMemberFloat mana; //0x0204
    CMemberFloat manaMaxBase; //0x0214
    CMemberFloat manaMaxMod; //0x0224
    CMemberFloat stamina; //0x0234
    CMemberFloat staminaMaxBase; //0x0244
    CMemberFloat staminaMaxMod; //0x0254
    CMemberFloat food; //0x0264
    CMemberFloat foodMinRatio; //0x0274
    CMemberUShort level; //0x0284
    CMemberULong cumulatedLevel; //0x0290
    CMemberUShort maxLevel; //0x02A0
    CMemberUShort rebirthCount; //0x02AC
    CMemberUShort lifeTimeSkill; //0x02B8
    char pad_02C4[4]; //0x02C4
    CMemberUInt64 experienceInt; //0x02C8
    CMemberUShort age; //0x02E0
    char pad_02EC[1880]; //0x02EC
    CMemberFloat dorcha; //0x0A44
    CMemberFloat dorchaMaxBase; //0x0A54
    CMemberFloat tuairim; //0x0A64 For Bachram Boost
    CMemberFloat tuairimMaxBase; //0x0A74 For Bachram Boost
    char pad_0A84[56]; //0x0A84
}; //Size: 0x0ABC

class CFramework
{
public:
    char pad_0000[152]; //0x0000
    Vector3f position; //0x0098
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
    char pad_0000[12]; //0x0000
    uint32_t classID; //0x000C
    char pad_0010[12]; //0x0010
    uint32_t color1; //0x001C
    uint32_t color2; //0x0020
    uint32_t color3; //0x0024
    uint32_t color4; //0x0028
    uint32_t color5; //0x002C
    uint32_t color6; //0x0030
    char pad_0034[20]; //0x0034
}; //Size: 0x0048

class CEquipment
{
public:
    CEquipmentItemInfo itemInfo[19]; //0x0000
    char pad_0558[384]; //0x0558
}; //Size: 0x06D8

class CCharacter
{
public:
    char pad_0000[4]; //0x0000
    class CEntityID* entityID; //0x0004
    char pad_0008[128]; //0x0008
    class CParameter* parameter; //0x0088
    class CAction* action; //0x008C
    char pad_0090[412]; //0x0090
    class CEquipment* equipment; //0x022C
    char pad_0230[96]; //0x0230
    uint64_t targetID; //0x0290
    char pad_0298[700]; //0x0298
}; //Size: 0x0554

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
    char pad_0000[168]; //0x0000
    uint64_t localPlayerID; //0x00A8
    char pad_00B0[944]; //0x00B0
}; //Size: 0x0460

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

class CUI
{
public:
    char pad_0000[344]; //0x0000
    uint32_t chatTab; //0x0158
    char pad_015C[728]; //0x015C
}; //Size: 0x0434
