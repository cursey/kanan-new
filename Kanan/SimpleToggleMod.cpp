#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "SimpleToggleMod.hpp"

using namespace std;

namespace kanan {
	SimpleToggleMod::SimpleToggleMod(const char* configName, const char* patchName, const char* tooltip)
		: m_isEnabled{ false }
	{
		m_config_name = configName;
		m_patch_name = patchName;
		m_tooltip = tooltip;
	}

	bool SimpleToggleMod::addPatch(const char* scanBytes, int byteOffset, std::vector<int16_t> newBytes) {
		auto address = scan("client.exe", scanBytes);

		if (address) {
			Patch* patch = new Patch();
			patch->address = *address + byteOffset;
			patch->bytes = newBytes;
			m_patches.push_back(patch);
			return true;
		}
		else {
			return false;
		}
	}

	void SimpleToggleMod::onPatchUI() {
		if (m_patches.empty()) {
			return;
		}

		if (ImGui::Checkbox(m_patch_name, &m_isEnabled)) {
			apply();
		}

		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip(m_tooltip);
		}
	}

	void SimpleToggleMod::onConfigLoad(const Config& cfg) {
		m_isEnabled = cfg.get<bool>(m_config_name).value_or(false);

		apply();
	}

	void SimpleToggleMod::onConfigSave(Config& cfg) {
		cfg.set<bool>(m_config_name, m_isEnabled);
	}

	void SimpleToggleMod::apply() {
		if (m_patches.empty()) {
			return;
		}

		if (m_isEnabled) {
			applyPatches();
		}
		else {
			undoPatches();
		}
	}

	void SimpleToggleMod::applyPatches() {
		for (Patch* p : m_patches) {
			if (p->address != 0) {
				patch(*p);
			}
		}
	}

	void SimpleToggleMod::undoPatches() {
		for (Patch* p : m_patches) {
			if (p->address != 0) {
				undoPatch(*p);
			}
		}
	}
}
