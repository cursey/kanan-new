#include <algorithm>

#include <imgui.h>

#include <Scan.hpp>

#include "Kanan.hpp"
#include "Log.hpp"
#include "EquipmentOverride.hpp"

using namespace std;

namespace kanan {
    static EquipmentOverride* g_equipmentOverride{ nullptr };
	static bool g_equipmentOverrideOnLoad;

	static int convertFloatColorToInt(const array<float, 4>& color) {
		auto r = (uint8_t)(255 * color[0]);
		auto g = (uint8_t)(255 * color[1]);
		auto b = (uint8_t)(255 * color[2]);
		auto a = (uint8_t)(255 * color[3]);
		return (a << 24) + (r << 16) + (g << 8) + b;
	};

	static array<float, 4> convertIntColorToFloat(int color) {
        auto r = color & 0x00FF0000;
        auto g = color & 0x0000FF00;
        auto b = color & 0x000000FF;
        auto a = color & 0xFF000000;

		return {
			clamp((float)r / 255.0f, 0.0f, 1.0f),
			clamp((float)g / 255.0f, 0.0f, 1.0f),
			clamp((float)b / 255.0f, 0.0f, 1.0f),
			clamp((float)a / 255.0f, 0.0f, 1.0f)
		};
	};

    int convertInventoryIDToEquipmentSlot(int inventoryID) {
        /*static auto fn = (int(__cdecl*)(int))scan("client.exe", "55 8B EC 8B 45 08 83 C0 ? 83 F8 ? 77 ? 0F B6 80 CC 18 69 01").value_or(0);
        static bool logged{ false };

        if (!logged) {
            log("[EquipmentOverride] convertInventoryIDToEquipmentSlot %p", fn);
            logged = true;
        }

        return (fn != nullptr) ? fn(inventoryID) : 0;*/

        int a1 = inventoryID;

        // Tired of the patterns breaking so here's the function strait from IDA.
        signed int result; // eax

        switch (a1)
        {
        case 4294967294:
            result = 9;
            break;
        case 3:
            result = 6;
            break;
        case 4:
            result = 7;
            break;
        case 5:
        case 43:
            result = 1;
            break;
        case 6:
        case 44:
            result = 4;
            break;
        case 7:
        case 45:
            result = 5;
            break;
        case 8:
        case 46:
            result = 2;
            break;
        case 9:
        case 47:
            result = 8;
            break;
        case 10:
            result = 10;
            break;
        case 11:
            result = 11;
            break;
        case 12:
        case 14:
            result = 12;
            break;
        case 13:
        case 15:
            result = 13;
            break;
        case 16:
            result = 16;
            break;
        case 17:
            result = 17;
            break;
        case 48:
            result = 15;
            break;
        case 90:
            result = 18;
            break;
        default:
            result = -1;
            break;
        }
        return result;
    }

    EquipmentOverride::EquipmentOverride()
        : m_equipmentOverrides{},
        m_setEquipmentInfoHook{},
        m_isNoFlashyEquipmentEnabled{}
    {
        log("[EquipmentOverride] Entering constructor");

        g_equipmentOverride = this;

        // Set the names for the equipment slots.
        m_equipmentOverrides[1].name = "Torso\\Armor\\Shirt";
        m_equipmentOverrides[2].name = "Head\\Helmet\\Hat";
        m_equipmentOverrides[4].name = "Hands\\Gauntlets\\Gloves";
        m_equipmentOverrides[5].name = "Feet\\Boots\\Shoes";
        m_equipmentOverrides[7].name = "Hair";
        m_equipmentOverrides[8].name = "Back\\Wings\\Robe";
        m_equipmentOverrides[10].name = "Weapon 1";
        m_equipmentOverrides[11].name = "Weapon 2";
        m_equipmentOverrides[12].name = "Arrow\\Shield 1";
        m_equipmentOverrides[13].name = "Arrow\\Shield 2";
        m_equipmentOverrides[15].name = "Tail";
        m_equipmentOverrides[16].name = "Accessory 1";
        m_equipmentOverrides[17].name = "Accessory 2";

        auto address = scan("client.exe", "55 8B EC 6A ? 68 ? ? ? ? 64 A1 ? ? ? ? 50 83 EC ? 53 56 57 A1 ? ? ? ? 33 C5 50 8D 45 F4 64 A3 ? ? ? ? 8B F9 8B 45 0C 8B 0D ? ? ? ?");

        if (address) {
            log("[EquipmentOverride] Found address of setEquipmentInfo %p", *address);

            m_setEquipmentInfoHook = make_unique<FunctionHook>(*address, (uintptr_t)&EquipmentOverride::hookedSetEquipmentInfo);
        }
        else {
            log("[EquipmentOverride] Failed to find address of setEquipmentInfo!");
        }

        log("[EquipmentOverride] Leaving constructor");
    }

    EquipmentOverride::~EquipmentOverride() {
        m_setEquipmentInfoHook->remove();
        g_equipmentOverride = nullptr;
    }

    void EquipmentOverride::onUI() {
        if (ImGui::CollapsingHeader("Equipment Override")) {
            ImGui::TextWrapped("After enabling an override you must re-equip an item in that slot to see the changes. "
                "NOTE: For the Hair slot, you need to change your hair in the dressing room to see the changes.");
            ImGui::Spacing();
            ImGui::Text("Special thanks to Rydian!");
			ImGui::Checkbox("Enable override on game load", &g_equipmentOverrideOnLoad);
			ImGui::Spacing();

            for (auto& overrideInfo : m_equipmentOverrides) {
                if (overrideInfo.name.empty()) {
                    continue;
                }

                if (ImGui::TreeNode(&overrideInfo, "%s", overrideInfo.name.c_str())) {
                    ImGui::Checkbox("Enable color override", &overrideInfo.isOverridingColor);
                    ImGui::ColorEdit4("Color 1", overrideInfo.color1.data(), ImGuiColorEditFlags_HEX);
                    ImGui::ColorEdit4("Color 2", overrideInfo.color2.data(), ImGuiColorEditFlags_HEX);
                    ImGui::ColorEdit4("Color 3", overrideInfo.color3.data(), ImGuiColorEditFlags_HEX);
                    ImGui::Checkbox("Enable item override", &overrideInfo.isOverridingItem);
                    ImGui::InputInt("Item ID", (int*)&overrideInfo.itemID);
                    ImGui::TreePop();
                }
            }
        }

        if (ImGui::CollapsingHeader("No Flashy Equipment")) {
            ImGui::TextWrapped("Disables flashy equipment dyes for all players. "
                "After enabling or disabling this option you should change channels to see the effects on everyone.");
            ImGui::Checkbox("Enable No Flashy Equipment", &m_isNoFlashyEquipmentEnabled);
        }
    }

    void EquipmentOverride::onConfigLoad(const Config& cfg) {
		m_isNoFlashyEquipmentEnabled = cfg.get<bool>("NoFlashyEquipment.Enabled").value_or(false);
		g_equipmentOverrideOnLoad = cfg.get<bool>("EquipmentOverrideOnLoad.Enabled").value_or(false);

		auto j = 0;

		for (auto& overrideInfo : m_equipmentOverrides) {
			overrideInfo.isOverridingColor = cfg.get<bool>("EquipmentOverride." + to_string(j) + ".IsOverridingColor").value_or(false);
			overrideInfo.itemID = cfg.get<int>("EquipmentOverride." + to_string(j) + ".ItemID").value_or(0);
			overrideInfo.color1 = convertIntColorToFloat(cfg.get<int>("EquipmentOverride." + to_string(j) + ".Color1").value_or(0));
			overrideInfo.color2 = convertIntColorToFloat(cfg.get<int>("EquipmentOverride." + to_string(j) + ".Color2").value_or(0));
			overrideInfo.color3 = convertIntColorToFloat(cfg.get<int>("EquipmentOverride." + to_string(j) + ".Color3").value_or(0));

            if (g_equipmentOverrideOnLoad) {
                overrideInfo.isOverridingItem = cfg.get<bool>("EquipmentOverride." + to_string(j) + ".IsOverridingItem").value_or(false);
            }
            else {
                overrideInfo.isOverridingItem = false;
            }

			++j;
		}
    }

    void EquipmentOverride::onConfigSave(Config& cfg) {
		cfg.set<bool>("NoFlashyEquipment.Enabled", m_isNoFlashyEquipmentEnabled);
		cfg.set<bool>("EquipmentOverrideOnLoad.Enabled", g_equipmentOverrideOnLoad);

		auto j = 0;

		for (auto& overrideInfo : m_equipmentOverrides) {
			cfg.set<bool>("EquipmentOverride." + to_string(j) + ".IsOverridingColor", overrideInfo.isOverridingColor);
			cfg.set<int>("EquipmentOverride." + to_string(j) + ".ItemID", overrideInfo.itemID);
			cfg.set<int>("EquipmentOverride." + to_string(j) + ".Color1", convertFloatColorToInt(overrideInfo.color1));
			cfg.set<int>("EquipmentOverride." + to_string(j) + ".Color2", convertFloatColorToInt(overrideInfo.color2));
			cfg.set<int>("EquipmentOverride." + to_string(j) + ".Color3", convertFloatColorToInt(overrideInfo.color3));
			cfg.set<bool>("EquipmentOverride." + to_string(j) + ".IsOverridingItem", g_equipmentOverrideOnLoad ? overrideInfo.isOverridingItem : false);

			++j;
		}
    }

    void EquipmentOverride::hookedSetEquipmentInfo(CEquipment* equipment, uint32_t EDX, int inventoryID, int itemID, int a4, int a5, uint32_t* color, int a7, int * a8, int a9, int a10, int * a11) {
        auto orig = (decltype(hookedSetEquipmentInfo)*)g_equipmentOverride->m_setEquipmentInfoHook->getOriginal();
        auto equipmentSlot = convertInventoryIDToEquipmentSlot(inventoryID);

        // Remove the flashy byte if No Flashy Equipment is enabled. We skip the 
        // hair slot.
        if (g_equipmentOverride->m_isNoFlashyEquipmentEnabled && equipmentSlot != 7) {
            color[0] &= 0x00FFFFFF;
            color[1] &= 0x00FFFFFF;
            color[2] &= 0x00FFFFFF;
        }

        // Filter out other characters.
        auto game = g_kanan->getGame();
        auto localCharacter = game->getLocalCharacter();

        if (localCharacter == nullptr || equipment != localCharacter->equipment) {
            return orig(equipment, EDX, inventoryID, itemID, a4, a5, color, a7, a8, a9, a10, a11);
        }

        // Filter out inventoryIDs.
        if (equipmentSlot < 0 || equipmentSlot >= (int)g_equipmentOverride->m_equipmentOverrides.size()) {
            return orig(equipment, EDX, inventoryID, itemID, a4, a5, color, a7, a8, a9, a10, a11);
        }

        auto& overrideInfo = g_equipmentOverride->m_equipmentOverrides[equipmentSlot];

        if (overrideInfo.isOverridingColor) {

            color[0] = convertFloatColorToInt(overrideInfo.color1);
            color[1] = convertFloatColorToInt(overrideInfo.color2);
            color[2] = convertFloatColorToInt(overrideInfo.color3);

            log("[EquipmentOverride] Color overwritten!");
        }

        if (overrideInfo.isOverridingItem) {
            itemID = overrideInfo.itemID;

            log("[EquipmentOverride] Item overwritten!");
        }

        return orig(equipment, EDX, inventoryID, itemID, a4, a5, color, a7, a8, a9, a10, a11);
    }
}
