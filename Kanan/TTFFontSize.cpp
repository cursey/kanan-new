#include <imgui.h>

#include <Scan.hpp>

#include "Log.hpp"
#include "TTFFontSize.hpp"
#include <stdlib.h>

using namespace std;

namespace kanan {
	TTFFontSize::TTFFontSize()
		: PatchMod{ "TTF Font Size", "" },
		m_size{},
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
		if (ImGui::InputInt("TTF Font Size", (int*)&m_size)) {
			if (m_size < 0) m_size = 0;
			else if (m_size > 255) m_size = 255;
			applyPatch();
		}
	}

	void TTFFontSize::onConfigLoad(const Config& cfg) {
		m_size = cfg.get<int>("TTFFontSize.Size").value_or(0);

		if (m_size != 0) {
			applyPatch();
		}
	}

	void TTFFontSize::onConfigSave(Config& cfg) {
		cfg.set<int>("TTFFontSize.Size", m_size);
	}

	void TTFFontSize::applyPatch() {
		if (m_patch.address == 0) {
			return;
		}

		log("Applying TTFFontSize...");
		short sizeByte = m_size;
		m_patch.bytes = { sizeByte };

		patch(m_patch);
	}
}