#include <algorithm>

#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "ZeroFogDistance.hpp"

using namespace std;

namespace kanan {
	ZeroFogDistance::ZeroFogDistance()
		: m_isEnabled{ false },
		m_Patch1{},
		m_Patch2{}
	{
		auto address1 = scan("client.exe", "D9 45 14 5D C2 10");
		if (!address1) {
			log("ZeroFogDistance Failed to find pattern %s", "D9 45 14 5D C2 10");
			return;
		}
		m_Patch1.address = *address1 + 1;
		m_Patch1.bytes = { 0xEB, 0x90 };

		auto address2 = scan("client.exe", m_Patch1.address, "D9 45 14 5D C2 10");
		if (!address2) {
			log("ZeroFogDistance Failed to find second pattern %s", "D9 45 14 5D C2 10");
			return;
		}
		m_Patch2.address = *address2 + 1;
		m_Patch2.bytes = { 0xEB, 0x90 };
	}

	void ZeroFogDistance::onPatchUI() {
		if (m_Patch1.address == 0|| m_Patch2.address == 0) {
			return;
		}

		if (ImGui::Checkbox("Zero Fog Distance", &m_isEnabled)) {
			apply();
		}

		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Stops fog from rendering in the distance. Works best when combied with RenderDistance");
		}
	}

	void ZeroFogDistance::onConfigLoad(const Config& cfg) {
		m_isEnabled = cfg.get<bool>("ZeroFogDistance.Enabled").value_or(false);

		if (m_isEnabled) {
			apply();
		}
	}

	void ZeroFogDistance::onConfigSave(Config& cfg) {
		cfg.set<bool>("ZeroFogDistance.Enabled", m_isEnabled);
	}

	void ZeroFogDistance::apply() {
		if (m_Patch1.address == 0 || m_Patch2.address == 0) {
			return;
		}

		log("[%s] Toggling %s", "ZeroFogDistance", m_isEnabled ? "on" : "off");

		if(m_isEnabled) {
			patch(m_Patch1);
			patch(m_Patch2);
		}
		else {
			undoPatch(m_Patch1);
			undoPatch(m_Patch2);
		}
	}
}
