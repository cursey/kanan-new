#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "FreeZoom.hpp"

using namespace std;

namespace kanan {
	// Description:
	// Removes zooming restrictions. Zoom in and zoom out as much as you want.

	//Walkthrough:
	// Find value that changes between 400.0,3000.0 as you zoom in and out.
	// Find what writes to this value. (fstp [esi+1Ch])
	// Change a jp and jnz above it to jmp.
	FreeZoom::FreeZoom()
		: m_isEnabled{ false },
		m_patch_jp{},
		m_patch_jnz{}
	{
		auto address = scan("client.exe", "7A 05 D9 5D 08 EB 02 DD D8 D9 45 08 D9 45 F8");

		if (address) {
			log("Got FreeZoom address %p", *address);

			//We want to patch the jp and jnz to jmp
			m_patch_jp.address = *address;
			m_patch_jp.bytes = { 0xEB };

			m_patch_jnz.address = *address + 22;
			m_patch_jnz.bytes = { 0xEB };
		}
		else {
			log("Failed to find FreeZoom address.");
		}
	}

	void FreeZoom::onPatchUI() {
		if (m_patch_jp.address == 0 || m_patch_jnz.address == 0) {
			return;
		}

		if (ImGui::Checkbox("Free Zoom", &m_isEnabled)) {
			apply();
		}
	}

	void FreeZoom::onConfigLoad(const Config& cfg) {
		m_isEnabled = cfg.get<bool>("FreeZoom.Enabled").value_or(false);

		if (m_isEnabled) {
			apply();
		}
	}

	void FreeZoom::onConfigSave(Config& cfg) {
		cfg.set<bool>("FreeZoom.Enabled", m_isEnabled);
	}

	void FreeZoom::apply() {
		if (m_patch_jp.address == 0 || m_patch_jnz.address == 0) {
			return;
		}

		log("Toggling FreeZoom...");

		if (m_isEnabled) {
			patch(m_patch_jp);
			patch(m_patch_jnz);
		}
		else {
			undoPatch(m_patch_jp);
			undoPatch(m_patch_jnz);
		}
	}
}
