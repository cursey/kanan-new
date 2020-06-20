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

// Created with NetClass.RE 1.2 by KN4CK3R

class CRendererPtr
{
public:
	class CRenderer *renderer; //0x0000
	char pad_0004[60]; //0x0004
}; //Size: 0x0040

class CRenderer
{
public:
	char pad_0000[36]; //0x0000
	class CCamera *camera; //0x0024
	char pad_0028[28]; //0x0028
}; //Size: 0x0044

class CCamera
{
public:
	class CCameraState *state; //0x0000
	char pad_0004[64]; //0x0004
}; //Size: 0x0044

class CCameraState
{
public:
	char pad_0000[4]; //0x0000
	Vector3 target; //0x0004
	char pad_0010[8]; //0x0010
	Vector3 position; //0x0018
	char pad_0024[8]; //0x0024
	Vector4 forward; //0x002C
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
	Matrix4x4 viewMatrix; //0x014C
	char pad_018C[84]; //0x018C
}; //Size: 0x01E0

class CEntityListPtr
{
public:
	class CEntityList *entityList; //0x0000
	char pad_0004[60]; //0x0004
}; //Size: 0x0040

class CItemList
{
public:
	char pad_0000[4]; //0x0000
	class CItemListNode *root; //0x0004
	uint32_t count; //0x0008
}; //Size: 0x000C

class CCharacterList
{
public:
	char pad_0000[4]; //0x0000
	class CCharacterListNode *root; //0x0004
	uint32_t count; //0x0008
}; //Size: 0x000C

class CEntityList
{
public:
	char pad_0000[8]; //0x0000
	class CItemList items; //0x0008
	char pad_0014[20]; //0x0014
	class CCharacterList characters; //0x0028
	char pad_0034[84]; //0x0034
}; //Size: 0x0088

class CItemListNode
{
public:
	class CItemListNodeEntry *entry; //0x0000
	class CItemListNode *next; //0x0004
}; //Size: 0x0008

class CItemListNodeEntry
{
public:
	char pad_0000[16]; //0x0000
	class CItem *item; //0x0010
	char pad_0014[48]; //0x0014
}; //Size: 0x0044

class CCharacterListNode
{
public:
	class CCharacterListNodeEntry *entry; //0x0000
	class CCharacterListNode *next; //0x0004
}; //Size: 0x0008

class CCharacterListNodeEntry
{
public:
	char pad_0000[16]; //0x0000
	class CCharacter *character; //0x0010
	char pad_0014[48]; //0x0014
}; //Size: 0x0044

class CCharacter
{
public:
	char pad_0000[4]; //0x0000
	class CEntityID *entityID; //0x0004
	char pad_0008[128]; //0x0008
	class CParameter *parameter; //0x0088
	class CAction *action; //0x008C
	char pad_0090[432]; //0x0090
	class CEquipment *equipment; //0x0240
	char pad_0244[84]; //0x0244
	uint64_t targetID; //0x0298
	char pad_02A0[700]; //0x02A0
}; //Size: 0x055C

class CItem
{
public:
	char pad_0000[4]; //0x0000
	class CEntityID *entityID; //0x0004
	uint32_t inventoryID; //0x0008
	uint32_t itemID; //0x000C
	uint32_t color1; //0x0010
	uint32_t color2; //0x0014
	uint32_t color3; //0x0018
	char pad_001C[4]; //0x001C
	uint32_t color4; //0x0020
	uint32_t color5; //0x0024
	uint32_t color6; //0x0028
	uint32_t stackCount; //0x002C
	char pad_0030[4]; //0x0030
	uint32_t positionX; //0x0034
	uint32_t positionY; //0x0038
	char pad_003C[40]; //0x003C
	uint32_t price; //0x0064
	uint32_t sellPrice; //0x0068
	char pad_006C[4]; //0x006C
	uint32_t durability; //0x0070
	uint32_t maxDurability; //0x0074
	uint32_t originalDurability; //0x0078
	char pad_007C[92]; //0x007C
	class SItemDBDesc *dbDesc; //0x00D8
	char pad_00DC[28]; //0x00DC
	uint64_t ownerID; //0x00F8
	char pad_0100[36]; //0x0100
	class CString *name; //0x0124
	char pad_0128[56]; //0x0128
}; //Size: 0x0160

class CEntityID
{
public:
	uint64_t id; //0x0000
	char pad_0008[64]; //0x0008
}; //Size: 0x0048

class CMemberString
{
public:
	char pad_0000[4]; //0x0000
	class CString *value; //0x0004
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
	char pad_0000[32]; //0x0000
	class CMemberString name; //0x0020
	class CMemberString title; //0x0030
	class CMemberString engTitle; //0x0040
	class CMemberULong type; //0x0050
	class CMemberUChar skinColor; //0x0060
	class CMemberUShort eyeType; //0x006C
	class CMemberUChar eyeColor; //0x0078
	class CMemberUChar mouthType; //0x0084
	class CMemberULong status; //0x0090
	class CMemberULong statusEx; //0x00A0
	class CMemberULong statusEx2; //0x00B0
	class CMemberFloat scaleHeight; //0x00C0
	class CMemberFloat scaleFatness; //0x00D0
	class CMemberFloat scaleUpper; //0x00E0
	class CMemberFloat scaleLower; //0x00F0
	class CMemberULong regionID; //0x0100
	class CMemberULong positionX; //0x0110
	class CMemberULong positionY; //0x0120
	class CMmemberChar direction; //0x0130
	class CMemberULong battleState; //0x013C
	class CMemberUChar weaponSet; //0x014C
	class CMemberULong extra1; //0x0158
	class CMemberULong extra2; //0x0168
	class CMemberULong extra3; //0x0178
	class CMemberFloat combatPower; //0x0188
	class CMemberString motionType; //0x0198
	class CMemberUChar oddEyeLeftColor; //0x01A8
	class CMemberUChar oddEyeRightColor; //0x01B4
	class CMemberFloat life; //0x01C0
	class CMemberFloat lifeMaxBase; //0x01D0
	class CMemberFloat lifeDamaged; //0x01E0
	class CMemberFloat lifeMaxMod; //0x01F0
	class CMemberFloat mana; //0x0200
	class CMemberFloat manaMaxBase; //0x0210
	class CMemberFloat manaMaxMod; //0x0220
	class CMemberFloat stamina; //0x0230
	class CMemberFloat staminaMaxBase; //0x0240
	class CMemberFloat staminaMaxMod; //0x0250
	class CMemberFloat food; //0x0260
	class CMemberFloat foodMinRatio; //0x0270
	class CMemberUShort level; //0x0280
	class CMemberULong cumulatedLevel; //0x028C
	class CMemberUShort maxLevel; //0x029C
	class CMemberUShort rebirthCount; //0x02A8
	class CMemberUShort lifeTimeSkill; //0x02B4
	class CMemberUInt64 experienceInt; //0x02C0
	class CMemberUShort age; //0x02D8
	char pad_02E4[1912]; //0x02E4
	class CMemberFloat dorcha; //0x0A5C
	class CMemberFloat dorchaMaxBase; //0x0A6C
	class CMemberFloat tuairim; //0x0A7C For Bachram Boost
	class CMemberFloat tuairimMaxBase; //0x0A8C For Bachram Boost
	char pad_0A9C[56]; //0x0A9C
}; //Size: 0x0AD4

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

class CAction
{
public:
	char pad_0000[64]; //0x0000
	class CCharacter *character; //0x0040
	class CAnimatorContext *animatorContext; //0x0044
	char pad_0048[1020]; //0x0048
}; //Size: 0x0444

class CAnimatorContext
{
public:
	char pad_0000[4]; //0x0000
	class CFramework *framework; //0x0004
	char pad_0008[60]; //0x0008
}; //Size: 0x0044

class CFramework
{
public:
	char pad_0000[152]; //0x0000
	Vector3 position; //0x0098
	char pad_00A4[168]; //0x00A4
}; //Size: 0x014C

class CWorldPtr
{
public:
	class CWorld *world; //0x0000
	char pad_0004[60]; //0x0004
}; //Size: 0x0040

class CWorld
{
public:
	char pad_0000[168]; //0x0000
	uint64_t localPlayerID; //0x00A8
	char pad_00B0[944]; //0x00B0
}; //Size: 0x0460

class SItemDBDesc
{
public:
	char pad_0000[40]; //0x0000
	class CString *type; //0x0028
	char pad_002C[8]; //0x002C
	class CString *displayName; //0x0034
	char pad_0038[4]; //0x0038
	class CString *name; //0x003C
	class CString *description; //0x0040
	char pad_0044[136]; //0x0044
	uint16_t maxStackCount; //0x00CC
	char pad_00CE[106]; //0x00CE
}; //Size: 0x0138

class CAccountPtr
{
public:
	class CAccount *account; //0x0000
	char pad_0004[60]; //0x0004
}; //Size: 0x0040

class CAccount
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044

class CEquipmentItemInfo
{
public:
	char pad_0000[12]; //0x0000
	uint32_t classID; //0x000C
	char pad_0010[12]; //0x0010
	uint32_t color1; //0x001C
	uint32_t color2; //0x0020
	uint32_t color3; //0x0024
	char pad_0028[4]; //0x0028
	uint32_t color4; //0x002C
	uint32_t color5; //0x0030
	uint32_t color6; //0x0034
	char pad_0038[24]; //0x0038
}; //Size: 0x0050

class CEquipment
{
public:
	class CEquipmentItemInfo itemInfo[19]; //0x0000
	char pad_05F0[384]; //0x05F0
}; //Size: 0x0770

class CUI
{
public:
	char pad_0000[344]; //0x0000
	uint32_t chatTab; //0x0158
	char pad_015C[728]; //0x015C
}; //Size: 0x0434

class N00000B19
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044

class N00000A4D
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
