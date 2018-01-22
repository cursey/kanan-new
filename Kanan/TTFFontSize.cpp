#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "TTFFontSize.hpp"

using namespace std;

namespace kanan {
	TTFFontSize::TTFFontSize()
		: PatchMod{ "TTF Font Size", "" },
		m_choice{},
		m_patch{},
		m_originalByte{}
	{
		auto address = scan("client.exe", "83 46 10 FC C7 86 A4 00 00 00 ? 00 00 00");

		if (address) {
			log("Got TTFFontSize %p", *address);

			m_patch.address = *address + 10;
			m_originalByte = { *reinterpret_cast<unsigned char*>(m_patch.address) };
		}
		else {
			log("Failed to find TTFFontSize address.");
		}
	}

	void TTFFontSize::onPatchUI() {
		if (m_patch.address == 0) {
			return;
		}

		ImGui::Text("TTF Original Font Size: %d", m_originalByte);
		if (ImGui::InputInt("TTF Font Size", (int*)&m_choice)) {
			if (m_choice < 0) m_choice = 0;
			else if (m_choice > 255) m_choice = 255;
			apply();
		}
	}

	void TTFFontSize::onConfigLoad(const Config& cfg) {
		m_choice = cfg.get<int>("TTFFontSize.Choice").value_or(0);

		if (m_choice != 0) {
			apply();
		}
	}

	void TTFFontSize::onConfigSave(Config& cfg) {
		cfg.set<int>("TTFFontSize.Choice", m_choice);
	}

	void TTFFontSize::apply() {
		if (m_patch.address == 0) {
			return;
		}

		log("Applying TTFFontSize...");
		m_patch.bytes = { m_choice };

		patch(m_patch);
	}
}